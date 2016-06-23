//
//  EEHSVShader.frag
//  ee-library
//
//  Created by Zinge on 6/22/16.
//
//

constexpr const char* ee_hsv_shader_frag = "\
#ifdef GL_ES                                                                 \n\
precision mediump float;                                                     \n\
#endif                                                                       \n\
                                                                             \n\
varying vec4 v_fragmentColor;                                                \n\
varying vec2 v_texCoord;                                                     \n\
                                                                             \n\
uniform mat4 u_hsv;                                                          \n\
                                                                             \n\
void main() {                                                                \n\
    vec4 pixelColor = texture2D(CC_Texture0, v_texCoord);                    \n\
                                                                             \n\
    // Store the original alpha.                                             \n\
    float alpha = pixelColor.w;                                              \n\
                                                                             \n\
    // Reset alpha to 1.0.                                                   \n\
    pixelColor.w = 1.0;                                                      \n\
                                                                             \n\
    vec4 fragColor = u_hsv * pixelColor;                                     \n\
                                                                             \n\
    // Restore the original alpha.                                           \n\
    fragColor.w = alpha;                                                     \n\
    gl_FragColor = fragColor * v_fragmentColor;                              \n\
}                                                                            \n\
";
