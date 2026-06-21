#pragma once

#include "platform/GL.h"
#include "renderer/backend/opengl/OpenGLState.h"

namespace ax {
namespace GL {
    inline void activeTexture(GLenum texture) {
        if (backend::__gl) {
            backend::__gl->activeTexture(texture);
        } else {
            glActiveTexture(texture);
        }
    }

    inline void bindTexture2D(GLuint textureId) {
        if (backend::__gl) {
            backend::__gl->bindTexture(GL_TEXTURE_2D, textureId);
        } else {
            glBindTexture(GL_TEXTURE_2D, textureId);
        }
    }

    inline void bindTexture2DN(GLuint index, GLuint textureId) {
        activeTexture(GL_TEXTURE0 + index);
        bindTexture2D(textureId);
    }

    inline void deleteTexture(GLuint textureId) {
        if (backend::__gl) {
            backend::__gl->deleteTexture(GL_TEXTURE_2D, textureId);
        } else {
            glDeleteTextures(1, &textureId);
        }
    }

    inline void useProgram(GLuint programId) {
        if (backend::__gl) {
            backend::__gl->useProgram(programId);
        } else {
            glUseProgram(programId);
        }
    }

    inline void enableVertexAttribs(unsigned int flags) {
        if (backend::__gl) {
            backend::__gl->disableUnusedVertexAttribs(flags);
            for (int i = 0; i < 16; ++i) {
                if (flags & (1 << i)) {
                    backend::__gl->enableVertexAttribArray(i);
                }
            }
        } else {
            for (int i = 0; i < 16; ++i) {
                if (flags & (1 << i)) {
                    glEnableVertexAttribArray(i);
                } else {
                    glDisableVertexAttribArray(i);
                }
            }
        }
    }

    inline void blendResetToCache() {
        // No-op or we can reset to default blend state if needed
    }
}
}
