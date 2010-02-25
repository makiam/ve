attribute vec3 vp_Vertex;
attribute vec3 vp_Color;

uniform mat4 vp_ProjectionMatrix;
uniform mat4 vp_ModelMatrix;
uniform mat4 vp_ViewMatrix;
uniform mat4 vp_ModelViewMatrix;
varying float fogFactor;

void main()
{
	gl_FrontColor = vec4(vp_Color, 1.0);
	gl_Position = vp_ProjectionMatrix * vp_ModelViewMatrix * vec4(vp_Vertex, 1.0);
	
	const float LOG2 = 1.442695;
	gl_FogFragCoord = length(vp_ModelViewMatrix * vec4(vp_Vertex, 1.0));
	fogFactor = exp2( -gl_Fog.density * 
					   gl_Fog.density * 
					   gl_FogFragCoord * 
					   gl_FogFragCoord * 
					   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
} 
