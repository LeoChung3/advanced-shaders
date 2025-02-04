#version 460 core

layout(location = 0) out vec4 colour;


in vec2 texCoord;


struct directionalLight
{
	vec3 colour;
	vec3 direction;
};

struct pointLight
{
	vec3 colour;
	vec3 position;
	vec3 constants;
};

struct spotLight
{
	vec3 colour;
	vec3 position;
	vec3 direction;
	vec3 constants;
	float cutOff;
	float outerCutOff;
};

const int numPointLights = 10;
const int numSpotLights = 1;

layout (std140, binding = 2) uniform b_lights
{
	uniform directionalLight dLight;
	uniform pointLight pLights[numPointLights];
	uniform spotLight sLights[numSpotLights];
};

layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

uniform sampler2D gBufferPosWs;
uniform sampler2D gBufferNormals;
uniform sampler2D gBufferDiffSpec;

uniform sampler2D skyboxstuff;
uniform sampler2D depthstuff;


uniform sampler2D u_shadowMap;
uniform mat4 u_lightSpaceTransform;



vec3 getDirectionalLight(vec3 worldPosition) ;
float ShadowCalculation(vec3 worldPosition);

vec3 getPointLight(int idx) ;
vec3 getSpotLight(int idx) ;


//global vars
vec3 normal ;
vec3 viewDir ;


vec3 posInWs;
vec3 n;
vec3 albedoColour ;
float specularStrength  = 0.8 ;

float depth;
vec3 skybox;

float PI = 3.14159;
uniform float u_metallic;
uniform float u_roughness;
uniform float ambientStrength;
uniform int pointLightOn;
void main()
{
	//vec3 result = vec3(0.0, 0.0, 0.0); 

	posInWs =			texture(gBufferPosWs, texCoord).rgb;
 //   viewDir = normalize(u_viewPos - posInWs);


	//normal =					texture(gBufferNormals, texCoord).rgb;
	//albedoColour =			texture(gBufferDiffSpec, texCoord).rgb;
	//specularStrength =	texture(gBufferDiffSpec, texCoord).a;

	depth = texture(depthstuff, texCoord).r;
	skybox = texture(skyboxstuff, texCoord).rgb;
	//
	////normal = normalize(n);

	//result += getDirectionalLight(posInWs);
	//
	//for(int i = 0; i <numPointLights; i++)
	//{
	//	//result += getPointLight(i);
	//}
	//
	//for(int i = 0; i <numSpotLights; i++)
	//{
	//	//result += getSpotLight(i);
	//}


	//// specular only    
	//
	//if (depth >= .9999f) colour = vec4(skybox, 1.0);
	//else
	//{

	//colour = vec4(result * specularStrength, 1.0) * vec4(albedoColour, 1.0);
	//}



	vec2 uv = texCoord;

	normal = texture(gBufferNormals, texCoord).rgb;
	//albedoColour =	(vec3(1));
	albedoColour =			texture(gBufferDiffSpec, texCoord).rgb;

	viewDir = normalize(u_viewPos - posInWs);

	vec3 result = vec3(0.0);
	result += getDirectionalLight(posInWs);

	for(int i = 0; i <numPointLights; i++)
	{
		if(pointLightOn == 1)
		{
			result += getPointLight(i);
		}
	}
	if (depth >= .9999f) colour = vec4(skybox, 1.0);
	else
	{
		colour = vec4(result, 1.0);
	}

	//colour = vec4(result, 1.0);

	// has albedo and specular for some reason???
	//colour = vec4(result * specularStrength, 1.0) * texture(u_albedoMap, texCoord);
	

}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(float NdotH)
{
	float a = u_roughness * u_roughness;
	float a2 = a * a;
	float NdotH2 = NdotH * NdotH;

	float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
	denominator = PI * denominator * denominator;

	return a2 / denominator;
}

float GeometrySchlickGGX(float Ndot)
{
	float r = (u_roughness + 1.0);
	float k = (r * r) / 8.0;
	float denominator = Ndot * (1.0 - k) + k;

	return Ndot / denominator;
}
float GeometrySmith(float NdotV, float NdotL)
{
	float ggx2 = GeometrySchlickGGX(NdotV);
	float ggx1 = GeometrySchlickGGX(NdotL);
	return ggx1 * ggx2;
}
vec3 getDirectionalLight(vec3 worldPosition)
{
	//float ambientStrength = 0.5;
	//vec3 ambient = ambientStrength * dLight.colour ;

	//float diffuseFactor = max(dot(normal, -dLight.direction), 0.0);
	//vec3 diffuse = diffuseFactor * dLight.colour ;
	//
	//vec3 H = normalize(-dLight.direction + viewDir ) ;
	//float specularFactor = pow(max(dot(normal, H) , 0.0), 64) ;
	//vec3 specular = dLight.colour * specularFactor * specularStrength;
	////float shadowAmount = ShadowCalculation();

	//float shadowAmount = ShadowCalculation(worldPosition);

 //   return ambient + (1.0 - shadowAmount)*(diffuse + specular);

	vec3 F0 = mix(vec3(0.04), albedoColour, u_metallic);
	vec3 L = normalize(-dLight.direction);
	vec3 H = normalize(L + viewDir);

	float NdotL = max(dot(normal, L), 0.0001);
	float NdotH = max(dot(normal, H), 0.0001);
	float NdotV = max(dot(normal, viewDir), 0.0001);
	vec3 F = fresnelSchlick(NdotH, F0);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - u_metallic;
	vec3 diffuse = (kD * albedoColour / PI) * NdotL;

	float D = DistributionGGX(NdotH);
	float G = GeometrySmith(NdotV, NdotL);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * NdotV * NdotL + 0.0001;
	vec3 specularStrength = numerator / denominator;


	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * vec3(1.0) * albedoColour;

	float specularFactor = pow(max(dot(normal, H), 0.0), 64);
	vec3 specular = dLight.colour * specularFactor * specularStrength;

	//return vec3(D);
	float shadowAmount = ShadowCalculation(worldPosition);
	return ambient + (1.0 - shadowAmount) * (specular + diffuse ) * (dLight.colour);
}

float ShadowCalculation(vec3 worldPosition)
{

	vec4 posInWsLightSpace = u_lightSpaceTransform * vec4(worldPosition, 1.0);

float shadow = 0.0;
vec3 projCoords = posInWsLightSpace.xyz / posInWsLightSpace.w;
projCoords = projCoords * 0.5 + 0.5;

float closestDepth = texture(u_shadowMap, projCoords.xy).r;
float currentDepth = projCoords.z;
float bias = 0.01;
if(currentDepth > (closestDepth + bias)){ shadow = 1.0;}
return shadow;
}

vec3 getPointLight(int idx)
{
	
	vec3 ambient = ambientStrength * pLights[idx].colour;

	float distance = length(pLights[idx].position - posInWs);
	float attn = 1.0/(pLights[idx].constants.x + (pLights[idx].constants.y* distance) + (pLights[idx].constants.z * (distance * distance)));
	vec3 lightDir = normalize(pLights[idx].position - posInWs);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * attn * pLights[idx].colour;

	vec3 H = normalize(-lightDir + viewDir ) ;
	float specularFactor = pow(max(dot(normal, H) , 0.0), 64) ;
    vec3 specular = pLights[idx].colour * specularFactor * specularStrength;
	
	return ambient + diffuse + specular;
}

vec3 getSpotLight(int idx)
{	
	vec3 lightDir = normalize(sLights[idx].position - posInWs);
	float theta = dot(lightDir, normalize(-sLights[idx].direction));
	float ambientStrength = 0.4 ;
	vec3 ambient = ambientStrength * sLights[idx].colour;

	if(theta > sLights[idx].outerCutOff)
	{

		float distance = length(sLights[idx].position - posInWs);
		float attn = 1.0/(sLights[idx].constants.x + (sLights[idx].constants.y* distance) + (sLights[idx].constants.z * (distance * distance)));
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = diff * attn * sLights[idx].colour;

		float specularStrength = 0.8;

		vec3 H = normalize(-lightDir + viewDir ) ;
	    float specularFactor = pow(max(dot(normal, H) , 0.0), 64) ;
        vec3 specular = pLights[idx].colour * specularFactor * specularStrength; 
	
		float epsilon = sLights[idx].cutOff - sLights[idx].outerCutOff;
		float intensity = clamp((theta - sLights[idx].outerCutOff) / epsilon, 0.0, 1.0); 
	
		diffuse  *= intensity;
		specular *= intensity;
		
		return (ambient + diffuse + specular);
	}
	else
	{
		return ambient;
	}
}

