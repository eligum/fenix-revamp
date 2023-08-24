#version 450 core

in vec3  fvertex;
in vec3  fnormal;
in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;

uniform vec3 posFocus;
uniform vec3 colFocus;

uniform int sLight;
uniform vec3 focusPos[3];
uniform vec3 focusCol[3];

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec4 FragColor;

vec3 llumAmbient = vec3(0.1, 0.1, 0.1);


vec3 Ambient()
{
    return llumAmbient * fmatamb;
}

vec3 Diffuse(vec3 NormSCO, vec3 L, vec3 sp_color)
{
    // Fixeu-vos que SOLS es retorna el terme de Lambert!
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0.0);
    if (dot(L, NormSCO) > 0)
        colRes = sp_color * fmatdiff * dot(L, NormSCO);
    return colRes;
}

vec3 Specular(vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 sp_color)
{
    // Fixeu-vos que SOLS es retorna el terme especular!
    // Assumim que els vectors estan normalitzats
    vec3 colRes = vec3(0.0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO, L) < 0) || (fmatshin == 0))
        return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: 2.0a * dot(NormSCO, L) * NormSCO - L;
    vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
        return colRes;  // no hi ha component especular

    float shine = pow(max(0.0, dot(R, V)), fmatshin);
    return (colRes + fmatspec * sp_color * shine);
}


void main()
{
    vec3 L;
    vec3 color = vec3(0.0);

    L     = normalize(posFocus - fvertex);
    color = Ambient() +
            Diffuse(normalize(fnormal), L, colFocus) +
            Specular(normalize(fnormal), L, fvertex, colFocus);

    FragColor = vec4(color, 1.0);
}
