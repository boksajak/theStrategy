#version 400

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

out vec2 TexCoord;

in BillboardData { 
        vec2 size;
		vec2 topLeftUV;
		vec2 bottomRightUV;
} billboardData[];  

void main(void)
{

	gl_Position = gl_in[0].gl_Position;
    TexCoord = billboardData[0].topLeftUV;
    EmitVertex();
    
	gl_Position = vec4(billboardData[0].size.x, 0.0, 0.0, 0.0) + gl_in[0].gl_Position;
    TexCoord = vec2(billboardData[0].bottomRightUV.x, billboardData[0].topLeftUV.y);
    EmitVertex();
    
	gl_Position = vec4(0.0, billboardData[0].size.y, 0.0, 0.0) + gl_in[0].gl_Position;
    TexCoord = vec2(billboardData[0].topLeftUV.x, billboardData[0].bottomRightUV.y);
    EmitVertex();
    
	gl_Position = vec4(billboardData[0].size.x, billboardData[0].size.y, 0.0, 0.0) + gl_in[0].gl_Position;
	TexCoord = billboardData[0].bottomRightUV;
	EmitVertex();

	EndPrimitive();

}
