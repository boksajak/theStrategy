#version 400 

uniform sampler2D bbTexture;
in vec2 TexCoord;

out vec4 outputColor;

void main(){    
    outputColor = texture(bbTexture, TexCoord).rgba;
}
