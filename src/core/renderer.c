#include "core/renderer.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_OPENGL_TEXTURE_SLOTS 32

typedef struct QuadVertex
{
	vec2_t pos;
	float id;
	vec2_t tex_uv;
	vec4_t color;
} QuadVertex;

typedef struct Renderer
{
	unsigned int vao, vbo, ibo;
	QuadVertex *vertex_data;
	int quads_to_render;
	int max_texture_slots;
	int textures_used;
	int samplers[MAX_OPENGL_TEXTURE_SLOTS];
	Texture *bound_textures[MAX_OPENGL_TEXTURE_SLOTS];
	Shader *batch_shader;
} Renderer;

static Renderer r; // renderer instance

void renderer_init(void)
{
	/* BATCH RENDERING BUFFERS */

	// vertex array

	glGenVertexArrays(1, &r.vao);
	glBindVertexArray(r.vao);

	// vertex buffer

	glGenBuffers(1, &r.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, r.vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * MAX_QUADS * sizeof(QuadVertex), NULL, GL_DYNAMIC_DRAW);

	r.vertex_data = malloc(4 * MAX_QUADS * sizeof(QuadVertex));

	// vertex buffer layout

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, sizeof(QuadVertex), 0);
	glEnableVertexAttribArray(1); // Texture id
	glVertexAttribPointer(1, 1, GL_FLOAT, 0, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, id));
	glEnableVertexAttribArray(2); // Texture uv
	glVertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, tex_uv));
	glEnableVertexAttribArray(3); // color
	glVertexAttribPointer(3, 4, GL_FLOAT, 0, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, color));

	// index buffer

	glGenBuffers(1, &r.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.ibo);

	// generate and set index buffer data

	unsigned int *index_buffer_data = malloc(6 * MAX_QUADS * sizeof(unsigned int));

	int n = 0;

	for (int i = 0; i < 6 * MAX_QUADS; i += 6)
	{
		index_buffer_data[i] = n;
		index_buffer_data[i + 1] = n + 1;
		index_buffer_data[i + 2] = n + 2;
		index_buffer_data[i + 3] = n + 2;
		index_buffer_data[i + 4] = n + 3;
		index_buffer_data[i + 5] = n;

		n += 4;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * MAX_QUADS * sizeof(unsigned int), index_buffer_data, GL_STATIC_DRAW);

	free(index_buffer_data);

	// the Shader by default

	r.batch_shader = shader_load("shaders/batch_rendering.vert", "shaders/batch_rendering.frag");

	// get Texture slots

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &r.max_texture_slots);

	for (int i = 0; i < r.max_texture_slots; i++)
		r.samplers[i] = i;
}

void renderer_destroy(void)
{
	glDeleteVertexArrays(1, &r.vao);
	glDeleteBuffers(1, &r.vbo);
	glDeleteBuffers(1, &r.ibo);
	free(r.vertex_data);
	shader_destroy(r.batch_shader);
}

void renderer_clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_viewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void renderer_viewport_ar(int window_width, int window_height, float target_aspect_ratio)
{
	// calculate target viewport size

	float window_aspect_ratio = window_width / (float)window_height;

	int target_x = 0;
	int target_y = 0;
	int target_width = window_width;
	int target_height = window_height;

	if (window_aspect_ratio > target_aspect_ratio)
	{
		target_width = target_height * target_aspect_ratio;
		target_x = (window_width - target_width) / 2;
	}
	else
	{
		target_height = target_width * (1 / target_aspect_ratio);
		target_y = (window_height - target_height) / 2;
	}

	glViewport(target_x, target_y, target_width, target_height);
}

void renderer_start_batch(const OrthoCamera *camera)
{
	shader_bind(r.batch_shader);
	shader_uniform_mat4(r.batch_shader, "u_projection", &camera->projection);
	shader_uniform_1iv(r.batch_shader, "u_textures", r.max_texture_slots, r.samplers);

	r.quads_to_render = 0;
	r.textures_used = 0;
}

void renderer_flush(void)
{
	if (r.quads_to_render > 0)
	{
		// use textures

		for (int i = 0; i < r.textures_used; i++)
			texture_use(r.bound_textures[i], i);

		// bind vbo and set data

		glBindBuffer(GL_ARRAY_BUFFER, r.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * r.quads_to_render * sizeof(QuadVertex), r.vertex_data);

		// bind vertex array

		glBindVertexArray(r.vao);

		// bind index buffer

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.ibo);

		// draw call

		glDrawElements(GL_TRIANGLES, 6 * r.quads_to_render, GL_UNSIGNED_INT, NULL);
	}

	r.quads_to_render = 0;
	r.textures_used = 0;
}

void renderer_texture(Texture *texture, vec2_t *pos, vec2_t *size, vec2_t *src_pos, vec2_t *src_size, vec4_t *color)
{
	if (r.quads_to_render >= MAX_QUADS || r.textures_used >= r.max_texture_slots)
	{
		renderer_flush();
	}

	// get Texture slot

	int slot = -1;

	for (int i = 0; i < r.textures_used; i++)
	{
		if (texture == r.bound_textures[i])
		{
			slot = i;
			break;
		}
	}

	if (slot == -1)
	{
		r.bound_textures[r.textures_used] = texture;
		slot = r.textures_used;
		r.textures_used++;
	}

	// Texture uv

	float src_x = src_pos->x / texture->width;
	float src_y = src_pos->y / texture->height;
	float src_w = src_size->x / texture->width;
	float src_h = src_size->y / texture->height;

	// set vertex data

	int index = r.quads_to_render * 4;

	r.vertex_data[index + 0] = (QuadVertex){VEC2(pos->x, pos->y + size->y), (float)slot, VEC2(src_x, 1 - (src_y + src_h)), *color};
	r.vertex_data[index + 1] = (QuadVertex){VEC2(pos->x + size->x, pos->y + size->y), (float)slot, VEC2(src_x + src_w, 1 - (src_y + src_h)), *color};
	r.vertex_data[index + 2] = (QuadVertex){VEC2(pos->x + size->x, pos->y), (float)slot, VEC2(src_x + src_w, 1 - src_y), *color};
	r.vertex_data[index + 3] = (QuadVertex){VEC2(pos->x, pos->y), (float)slot, VEC2(src_x, 1 - src_y), *color};

	// increment the number of quads

	r.quads_to_render++;
}

void renderer_model(const Model *model, const mat4_t *transform, const mat4_t *projection, const mat4_t *camera_transform, const Shader *shader)
{
	shader_bind(shader);
	shader_uniform_mat4(shader, "u_projection", projection);
	shader_uniform_mat4(shader, "u_model", transform);
	shader_uniform_mat4(shader, "u_view", camera_transform);

	// calculate normal matrix

	mat4_t normal_matrix, inverse;
	mat4_inverse(transform, &inverse);
	mat4_transpose(&inverse, &normal_matrix);

	shader_uniform_mat4(shader, "u_normal", &normal_matrix);

	// render each mesh of the model

	for (int i = 0; i < model->meshes_count; i++)
	{
		const Mesh *mesh = &model->meshes[i];
		const material_t *material = &mesh->material;

		shader_uniform_1i(shader, "u_texture_diffuse", 0);
		texture_use(material->texture_diffuse, 0);

		glBindVertexArray(mesh->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

		glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, NULL);
	}
}
