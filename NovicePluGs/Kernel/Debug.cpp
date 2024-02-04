#include "Debug.h"

#include <Novice.h>

#include "GameCamera.h"
#include "Renderer.h"
#include "Easing.h"

void Debug::BlackBakcground() {
	Vector2 topLeft = Transform2D::Homogeneous({ -1,1 }, GameCamera::instance->viewportMatrix);
	Vector2 bottomRight = Transform2D::Homogeneous({ 1,-1 }, GameCamera::instance->viewportMatrix);
	Novice::DrawBox(int(topLeft.x), int(topLeft.y), int(bottomRight.x - topLeft.x), int(bottomRight.y - topLeft.y), 0, BLACK, kFillModeSolid);
}

void Debug::Grid() {
	//Vector2 mid = Transform2D::Homogeneous(zeroVec2, GameCamera::instance->vpvMatrix);
	//Vector2 ver1 = Transform2D::Homogeneous(basisVec2X * 100, GameCamera::instance->vpvMatrix);
	//Vector2 ver2 = Transform2D::Homogeneous(basisVec2X * -100, GameCamera::instance->vpvMatrix);
	//Vector2 hol1 = Transform2D::Homogeneous(basisVec2Y * 100, GameCamera::instance->vpvMatrix);
	//Vector2 hol2 = Transform2D::Homogeneous(basisVec2Y * -100, GameCamera::instance->vpvMatrix);

	Vector2 topLeft = Transform2D::Homogeneous({ -1,1 }, GameCamera::instance->viewportMatrix);
	Vector2 bottomRight = Transform2D::Homogeneous({ 1,-1 }, GameCamera::instance->viewportMatrix);

	for (int i = 0; i <= 20; i++) {
		float pos = Lerp(-1.0f, 1.0f, i / 20.0f);
		Vector2 grid1 = Transform2D::Homogeneous({ 1, pos }, GameCamera::instance->viewportMatrix);
		Vector2 grid2 = Transform2D::Homogeneous({ -1, pos }, GameCamera::instance->viewportMatrix);
		Renderer::DrawLine(grid1, grid2, 0x404040ff);
		Vector2 grid3 = Transform2D::Homogeneous({ pos, -1 }, GameCamera::instance->viewportMatrix);
		Vector2 grid4 = Transform2D::Homogeneous({ pos, 1 }, GameCamera::instance->viewportMatrix);
		Renderer::DrawLine(grid3, grid4, 0x404040ff);
	}
	//Renderer::DrawLine({ ver1.x,mid.y }, { ver2.x, mid.y }, RED);
	//Renderer::DrawLine({ mid.x,hol1.y }, { mid.x, hol2.y }, BLUE);
}