//
// Created by holmes on 14/12/2020.
//

#include "draw_model.h"

void prepare_model(mesh_t *mesh) {
    glGenBuffers(1, &mesh->vertices_vbo);
    glGenBuffers(1, &mesh->texture_vbo);
    glGenBuffers(1, &mesh->normals_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh->count, mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->texture_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * mesh->count, mesh->texture_coordinates, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh->count, mesh->normals, GL_STATIC_DRAW);

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertices_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->texture_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->normals_vbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void render_model(mesh_t *mesh) {
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;

    /* View */
    get_view_matrix(view);
    glUniformMatrix4fv(glGetUniformLocation(mesh->shader, "view"), 1, GL_FALSE, &view[0][0]);

    /* Projection */
    glm_perspective(glm_rad(fov_back()), (float)800 / (float)600, 0.1f, 100.0f, projection);
    glUniformMatrix4fv(glGetUniformLocation(mesh->shader, "projection"), 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(mesh->vao);

    /* Model */
    glm_translate(model, mesh->position);

    glUniformMatrix4fv(glGetUniformLocation(mesh->shader, "model"), 1, GL_FALSE, &model[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->texture);

    glUseProgram(mesh->shader);
    glUniform1i(glGetUniformLocation(mesh->shader, "texture1"), 0);
    
    glDrawArrays(GL_TRIANGLES, 0, mesh->count);
}

void destroy_model(mesh_t *mesh) {
    glDeleteTextures(1, &mesh->texture);
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vertices_vbo);
    glDeleteBuffers(1, &mesh->texture_vbo);
    glDeleteBuffers(1, &mesh->normals_vbo);
}
