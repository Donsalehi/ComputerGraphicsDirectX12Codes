#include "d3dUtility.h"

using namespace std;

//
// Globals
//

D3DXMATRIX PositionFinder(float length, float previous_length, float objects_[],
			  float degree, string direction);

int n = 14;

IDirect3DDevice9* Device = 0;

const int Width = 1080;
const int Height = 720;

ID3DXMesh* Objects[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
D3DXMATRIX  Worlds[18];
D3DMATERIAL9 Mtrls[18];

//degree to rotation hands and legs
float rotation_degree = 0.0f;

//coordinates of body movement
float right_left_move = 0.0f; //coordinates of x
float up_down_move = 0.0f; //coordinates of y
float forward_backward_move = 0.0f; //coordinates of z

//speed of movement
float speed = 0.02f;

//last move
string last_move = "";

//do the hands move forward?
bool hands_move_forward = true;

//function for translating the matrix 
void translate(D3DXMATRIX* base, float x, float y, float z) {
	D3DXMATRIX translation_matrix;
	D3DXMatrixTranslation(&translation_matrix, x, y, z);
	*base *= translation_matrix;
}

//function for walking on the x axis
void walkX(string direction) {
	if (direction == "a") right_left_move += speed;
	if (direction == "d") right_left_move -= speed;

	// translate all objects
	if (direction == "a")
		for (int i = 0; i < 11 + 1; i++)
			translate(&Worlds[i], -speed, 0.0f, 0.0f);
	else
		for (int i = 0; i < 11 + 1; i++)
			translate(&Worlds[i], speed, 0.0f, 0.0f);
}

//function for walking on the y axis
void walkY(string direction) {
	if (direction == "q") up_down_move += speed;
	if (direction == "e") up_down_move -= speed;

	// translate all objects
	if (direction == "q")
		for (int i = 0; i < 11 + 1; i++)
			translate(&Worlds[i], 0.0f, -speed, 0.0f);
	else
		for (int i = 0; i < 11 + 1; i++)
			translate(&Worlds[i], 0.0f, speed, 0.0f);
}

//function for walking on the z axis
void walkZ(float degree, string direction) {
	if (direction == "w") forward_backward_move += speed;
	if (direction == "s") forward_backward_move -= speed;

	//rotate hands
	Worlds[6] = PositionFinder(3.0f, 0, new float[3] {-1.85f - right_left_move, -1.6f + up_down_move,
		0.0f - forward_backward_move}, rotation_degree, "x");
	Worlds[7] = PositionFinder(3.0f, 0, new float[3] {+1.85f - right_left_move, -1.6f + up_down_move,
		0.0f - forward_backward_move}, -rotation_degree, "x");

	Device->SetTransform(D3DTS_WORLD, &Worlds[6]);
	Objects[6]->DrawSubset(0);
	Device->SetTransform(D3DTS_WORLD, &Worlds[7]);
	Objects[7]->DrawSubset(0);

	//rotate legs
	Worlds[10] = PositionFinder(3.0f, 0, new float[3] {-1.0f - right_left_move, -4.8f + up_down_move,
		0.0f - forward_backward_move}, rotation_degree, "x");
	Worlds[11] = PositionFinder(3.0f, 0, new float[3] {1.0f - right_left_move, -4.8f + up_down_move,
		0.0f - forward_backward_move}, -rotation_degree, "x");

	Device->SetTransform(D3DTS_WORLD, &Worlds[10]);
	Objects[10]->DrawSubset(0);
	Device->SetTransform(D3DTS_WORLD, &Worlds[11]);
	Objects[11]->DrawSubset(0);

	// translate all objects
	if (direction == "w")
		for (int i = 0; i < 11 + 1; i++)
			translate(&Worlds[i], 0.0f, 0.0f, -speed);
	else
		for (int i = 0; i < 11 + 1; i++)
			translate(&Worlds[i], 0.0f, 0.0f, speed);
}

//
// Framework Functions
//
bool Setup()
{
	//
	// Create objects.
	//


	D3DXCreateBox(Device, 2.6, 4.1, 3.4, &Objects[0], 0); //body
	D3DXCreateSphere(Device, 1.0f, 20, 20, &Objects[1], 0); //head
	D3DXCreateSphere(Device, 0.1f, 20, 20, &Objects[2], 0); //noise
	D3DXCreateSphere(Device, 0.1f, 20, 20, &Objects[3], 0); //left eye
	D3DXCreateSphere(Device, 0.1f, 20, 20, &Objects[4], 0); //right eye
	D3DXCreateBox(Device, 0.6f, 0.1f, 0.1f, &Objects[5], 0); //mouth
	D3DXCreateBox(Device, 0.6f, 4.0f, 1.0f, &Objects[6], 0); //left feet
	D3DXCreateBox(Device, 0.6f, 4.0f, 1.0f, &Objects[7], 0); //right feet
	D3DXCreateBox(Device, 1.5f, 1.0f, 3.4f, &Objects[8], 0); //left shoulder
	D3DXCreateBox(Device, 1.5f, 1.0f, 3.4f, &Objects[9], 0); //right shoulder
	D3DXCreateBox(Device, 0.6f, 4.0f, 1.29f, &Objects[10], 0); //left hand
	D3DXCreateBox(Device, 0.6f, 4.0f, 1.29f, &Objects[11], 0); //right hand
	D3DXCreateBox(Device, 5000.0f, 0.0f, 5000.0f, &Objects[12], 0); //sky
	D3DXCreateBox(Device, 10000.0f, 0.0f, 5000.0f, &Objects[13], 0); //earth



	//
	// Build world matrices - position the objects in world space.
	//

	//body
	D3DXMatrixTranslation(&Worlds[0], 0.0f, 0.0f, 0.0f);

	//head
	D3DXMatrixTranslation(&Worlds[1], 0.0f, 3.0f, 0.0f);

	//noise
	D3DXMatrixTranslation(&Worlds[2], 0.0f, 3.15f, -1.0f);

	//left eye
	D3DXMatrixTranslation(&Worlds[3], -0.4f, 3.5f, -0.8f);

	//right eye
	D3DXMatrixTranslation(&Worlds[4], 0.35f, 3.5f, -0.75f);

	//mouth
	D3DXMatrixTranslation(&Worlds[5], -0.03f, 2.5f, -0.85f);

	//left feet
	D3DXMatrixTranslation(&Worlds[6], -0.99, -2.9, 0.0);

	//right feet
	D3DXMatrixTranslation(&Worlds[7], 0.99, -2.9, 0.0);

	//left sholder
	D3DXMatrixTranslation(&Worlds[8], -1.5, 1.55, 0.0);

	//right shoulder
	D3DXMatrixTranslation(&Worlds[9], 1.5, 1.55, 0.0);

	//left hand
	D3DXMatrixTranslation(&Worlds[10], -1.94, 0.0, 0.0);

	//right hand
	D3DXMatrixTranslation(&Worlds[11], 1.94, 0.0, 0.0);

	//sky
	D3DXMatrixTranslation(&Worlds[12], 0.0, 100.0, 1000.0);

	//earth
	D3DXMatrixTranslation(&Worlds[13], 0.0, -5.0, 0.0);

	//
	// Setup the object's materials.
	//

	Mtrls[0] = d3d::RED_MTRL;
	Mtrls[1] = d3d::BLUE_MTRL;
	Mtrls[2] = d3d::GREEN_MTRL;
	Mtrls[3] = d3d::YELLOW_MTRL;
	Mtrls[4] = d3d::YELLOW_MTRL;
	Mtrls[5] = d3d::WHITE_MTRL;
	Mtrls[6] = d3d::WHITE_MTRL;
	Mtrls[7] = d3d::WHITE_MTRL;
	Mtrls[8] = d3d::RED_MTRL;
	Mtrls[9] = d3d::RED_MTRL;
	Mtrls[10] = d3d::WHITE_MTRL;
	Mtrls[11] = d3d::WHITE_MTRL;
	Mtrls[12] = d3d::BLUE_MTRL;
	Mtrls[13] = d3d::GREEN_MTRL;


	//
	// Setup a directional light.
	//

	D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXCOLOR   c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);

	//
	// Set and Enable the light.
	//

	Device->SetLight(0, &dirLight);
	Device->LightEnable(0, true);

	//
	// Set lighting related render states.
	//

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, false);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);


	return true;
}

void Cleanup()
{
	for (int i = 0; i < n; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		// 
		// Update the scene: update camera position.
		//

		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float height = 5.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f * timeDelta;

		//move forward
		if (::GetAsyncKeyState(0x57) & 0x8000f) { // w
			if (last_move == "s") {
				while (rotation_degree >= 0) {
					forward_backward_move -= speed / 2.0f;
					walkZ(-forward_backward_move, "w");

					if (rotation_degree > 0)
						break;
				}
				while(rotation_degree < 0) {
					forward_backward_move += speed / 2.0f;
					walkZ(-forward_backward_move, "w");

					if (rotation_degree < 0)
						break;
				}
				last_move = "w";
			}
			else {
				last_move = "w";

				if (hands_move_forward)
					rotation_degree += speed;

				else
					rotation_degree -= speed;

				walkZ(rotation_degree, "w");

				if (rotation_degree >= 3.14f / 4)
					hands_move_forward = false;

				else if (rotation_degree <= -3.14f / 4)
					hands_move_forward = true;
			}
		}


		//move backward
		if (::GetAsyncKeyState(0x53) & 0x8000f) { // s
			if (last_move == "w") {
				while (rotation_degree >= 0) {
					forward_backward_move -= speed / 2.0f;
					walkZ(forward_backward_move, "s");

					if (rotation_degree > 0)
						break;
				}
				while (rotation_degree < 0) {
					forward_backward_move += speed / 2.0f;
					walkZ(forward_backward_move, "s");

					if (rotation_degree < 0)
						break;
				}
				last_move = "s";
			}
			else {
				last_move = "s";

				if (hands_move_forward)
					rotation_degree += speed;

				else
					rotation_degree -= speed;

				walkZ(-rotation_degree, "s");

				if (rotation_degree >= 3.14f / 4)
					hands_move_forward = false;

				else if (rotation_degree <= -3.14f / 4)
					hands_move_forward = true;
			}
		}

		//move left
		if (::GetAsyncKeyState(0x41) & 0x8000f) { // a
			walkX("a");
		}

		//move right
		if (::GetAsyncKeyState(0x44) & 0x8000f) { // d
			walkX("d");

		}

		//move up
		if (::GetAsyncKeyState(0x51) & 0x8000f) { // q
			walkY("q");
		}

		//move down
		if (::GetAsyncKeyState(0x45) & 0x8000f) { // e
			walkY("e");
		}

		D3DXVECTOR3 position(cosf(angle) * 7.0f, height, sinf(angle) * 7.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		Device->SetTransform(D3DTS_VIEW, &V);

		//
		// Draw the scene:
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		for (int i = 0; i < n; i++)
		{
			// set material and world matrix for ith object, then render
			// the ith object.
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTransform(D3DTS_WORLD, &Worlds[i]);
			Objects[i]->DrawSubset(0);
		}

		Device->EndScene();
		Device->Present(0, 0, 0, 0);

		return true;
	}
}


//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
static int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}
// function to find the position of objects after rotating in given axes
D3DXMATRIX PositionFinder(float length, float previous_length, float objects_[],
						  float degree, string direction) {

	D3DXMATRIX origin;
	D3DXMatrixIdentity(&origin);

	D3DXMATRIX rotator;
	if (direction == "x")
		rotator = D3DXMATRIX(1, 0, 0, 0,
							 0, cosf(degree), sinf(degree), 0,
							 0, -sinf(degree), cosf(degree), 0,
							 0, 0, 0, 1);

	else if (direction == "y")
		rotator = D3DXMATRIX(cosf(degree), 0, -sinf(degree), 0,
							 0, 1, 0, 0,
							 sinf(degree), 0, cosf(degree), 0,
							 0, 0, 0, 1);

	else
		rotator = D3DXMATRIX(cosf(degree), sinf(degree), 0, 0,
						     -sinf(degree), cosf(degree), 0, 0,
							 0, 0, 1, 0,
							 0, 0, 0, 1);


	D3DXMATRIX ultimate(1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						objects_[0], objects_[1] + previous_length + length / 2, objects_[2], 1);

	D3DXMATRIX result = origin * rotator * ultimate;
	return result;
}
