#version 450
#extension GL_ARB_separate_shader_objects : enable

/* Takes in a shader and applies the texture mapping to it using a sampler */

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() 
{
    vec4 textureValues = vec4(texture(texSampler, fragTexCoord));
    outColor = vec4(fragColor * textureValues.rgb, textureValues.a);
}
