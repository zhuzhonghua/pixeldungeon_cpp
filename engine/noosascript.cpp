#include "noosascript.h"
#include "game.h"
#include "quad.h"
#include "util.h"

const char* VERT_SHADER = R"(

uniform mat4 uCamera;
uniform mat4 uModel;
attribute vec4 aXYZW;
attribute vec2 aUV;
varying vec2 vUV;
void main() {
  gl_Position = uCamera * uModel * aXYZW;
  vUV = aUV;
}

)";

const char* FRAG_SHADER = R"(

precision mediump float;
varying vec2 vUV;
uniform sampler2D uTex;
uniform vec4 uColorM;
uniform vec4 uColorA;
void main() {
  gl_FragColor = texture2D( uTex, vUV ) * uColorM + uColorA;
}

)";

const char* VERT_SHADER_TEST = R"(

uniform mat4 uCamera;
uniform mat4 uModel;
attribute vec2 aXYZW;
attribute vec2 aUV;
varying vec2 vUV;
void main() {
	gl_Position.xy = (uCamera * uModel * vec4(aXYZW, 0, 1)).xy;
	//gl_Position.xy = aXYZW;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	vUV = aUV;
}

)";

const char* FRAG_SHADER_TEST = R"(

precision mediump float;
varying vec2 vUV;
uniform sampler2D uTex;
uniform vec4 uColorM;
uniform vec4 uColorA;
void main() {
	gl_FragColor = texture2D( uTex, vUV )* uColorM + uColorA;
	//gl_FragColor = vec4(1,0,0,1);
	//gl_FragColor = texture2D( uTex, vUV );
}

)";

NoosaScript::NoosaScript()
{
	compile(VERT_SHADER_TEST, FRAG_SHADER_TEST);

	uCamera = uniform("uCamera");
	uModel = uniform("uModel");
	uTex = uniform("uTex");
	uColorM = uniform("uColorM");
	uColorA = uniform("uColorA");
	aXY = attribute("aXYZW");
	aUV = attribute("aUV");
}

void NoosaScript::use()
{
	Script::use();

	aXY.enable();
	aUV.enable();
}

void NoosaScript::lighting(float rm, float gm, float bm, float am, float ra, float ga, float ba, float aa)
{
	uColorM.value4f(rm, gm, bm, am);
	uColorA.value4f(ra, ga, ba, aa);
}

NoosaScript* NoosaScript::get()
{
	Script* sc = Script::get<NoosaScript>();
	//if (sc == NULL)
	//{
	//	Script::_all.insert(std::make_pair("NoosaScript", new NoosaScript()));
	//	return dynamic_cast<NoosaScript*>(Script::get("NoosaScript"));
	//}

	return dynamic_cast<NoosaScript*>(sc);
}

void NoosaScript::camera(Camera* camera)
{
	if (camera == NULL)
	{
		camera = Camera::mainCamera;
	}

	if (camera != _lastCamera)
	{
		_lastCamera = camera;
		//uCamera.valueM4(camera->mat);

		glScissor(
			camera->x,
			Game::height - camera->screenHeightf - camera->y,
			camera->screenWidthf,
			camera->screenHeightf);
	}
}

void NoosaScript::drawQuad(float* buffer)
{
	uCamera.valueM4(_lastCamera->mat);
	aXY.vertexPointer(2, 4 * sizeof(float), buffer);
	aUV.vertexPointer(2, 4 * sizeof(float), buffer + 2);

	//std::vector<short>* indices = Quad::getIndices(1);
	//glDrawElements(GL_TRIANGLES, Quad::SIZE, GL_UNSIGNED_SHORT, (void*)(&((*indices)[0])));

	short indices[] = { 0, 1, 2, 0, 2, 3 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)indices);
}

void NoosaScript::drawQuadSet(float* buffer, int size)
{
	uCamera.valueM4(_lastCamera->mat);
	aXY.vertexPointer(2, 4 * sizeof(float), buffer);
	aUV.vertexPointer(2, 4 * sizeof(float), buffer + 2);

	static std::vector<short> indices;
	if (size*6 > indices.size())
	{
		indices.resize(size * 6);

		int pos = 0;
		int limit = size * 4;
		for (int ofs = 0; ofs < limit; ofs += 4) 
		{
			indices[pos++] = (short)(ofs + 0);
			indices[pos++] = (short)(ofs + 1);
			indices[pos++] = (short)(ofs + 2);
			indices[pos++] = (short)(ofs + 0);
			indices[pos++] = (short)(ofs + 2);
			indices[pos++] = (short)(ofs + 3);
		}
	}	
	glDrawElements(GL_TRIANGLES, 6*size, GL_UNSIGNED_SHORT, (void*)&indices[0]);
}

void NoosaScript::drawElements(FloatBuffer* vertices, ShortBuffer* indices, int size)
{
	vertices->position(0);
	aXY.vertexPointer(2, 4, vertices->buffer());

	vertices->position(2);
	aUV.vertexPointer(2, 4, vertices->buffer());

	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, indices->buffer());
}

