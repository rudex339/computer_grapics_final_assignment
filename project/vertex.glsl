#version 330 core

layout (location = 0) in vec3 aPos;   // ��ġ ������ attribute position 0�� �����ϴ�.
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 in_Color;
 out vec3 out_Color; //--- �����׸�Ʈ ���̴����� ����
uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;
void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPos, 1.0);
	out_Color = in_Color;
}  
