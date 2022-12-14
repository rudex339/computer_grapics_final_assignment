#version 330 core

in vec3 FragPos; //--- ��ְ��� ����ϱ� ���� ��ü�� ��ġ���� ���ؽ� ���̴����� �޾ƿ´�.
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos; //--- ������ ��ġ
uniform vec3 lightColor; //--- ������ ��
uniform vec3 objectColor;

void main ()
{
	float ambientLight = 0.5; //--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.0
	vec3 ambient = ambientLight * lightColor; //--- �ֺ� ����

	vec3 normalVector = normalize (Normal); //--- �븻���� ����ȭ�Ѵ�.
	vec3 lightDir = normalize(lightPos - FragPos);//--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
	float diffuseLight = max (dot (Normal, lightDir), 0.0); //--- N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.)
	vec3 diffuse = diffuseLight * lightColor; //--- ����ݻ�����=����ݻ簪*�������

	vec3 result = (ambient + diffuse) * objectColor; //--- ���� ���� ������ �ȼ� ����=(�ֺ�����+����ݻ�����)*��ü ����
	FragColor = vec4 (result, 1.0);

}