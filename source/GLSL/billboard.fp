#version 400 

uniform sampler2D bbTexture;
in vec2 TexCoord;

out vec4 outputColor;

// Render passes
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine (RenderPassType)
void NormalPass(){
    outputColor = texture(bbTexture, TexCoord).rgba;
}


subroutine (RenderPassType)
void StencilPass(){
	// Discard black pixels to create stencil buffer from BW texture
    if (texture(bbTexture, TexCoord).rgb == vec3(0)) discard;
    outputColor = vec4(1);
}

void main(){
    RenderPass();
}
