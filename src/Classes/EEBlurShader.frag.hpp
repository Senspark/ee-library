//
//  EEBlurShader.frag.hpp
//  ee-library
//
//  Created by Zinge on 6/24/16.
//
//

constexpr const char* ee_blur_shader_frag = "\
#ifdef GL_ES                                                                 \n\
precision mediump float;                                                     \n\
#endif                                                                       \n\
                                                                             \n\
#ifdef GL_ES                                                                 \n\
varying lowp vec4 v_fragmentColor;                                           \n\
varying mediump vec2 v_texCoord;                                             \n\
varying mediump vec2 v_blurTexCoords[4];                                     \n\
#else                                                                        \n\
varying vec4 v_fragmentColor;                                                \n\
varying vec2 v_texCoord;                                                     \n\
varying vec2 v_blurTexCoords[4];                                             \n\
#endif                                                                       \n\
                                                                             \n\
void main() {                                                                \n\
    const lowp float w0 = 0.2270270270;                                      \n\
    const lowp float w1 = 0.3162162162;                                      \n\
    const lowp float w2 = 0.0702702703;                                      \n\
                                                                             \n\
    gl_FragColor = vec4(0.0);                                                \n\
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[0]) * w2;         \n\
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[1]) * w1;         \n\
    gl_FragColor += texture2D(CC_Texture0, v_texCoord        ) * w0;         \n\
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[2]) * w1;         \n\
    gl_FragColor += texture2D(CC_Texture0, v_blurTexCoords[3]) * w2;         \n\
    gl_FragColor = gl_FragColor * v_fragmentColor;                           \n\
}                                                                            \n\
";