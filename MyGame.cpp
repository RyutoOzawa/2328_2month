#include "MyGame.h"
using namespace DirectX;

void MyGame::Initialize()
{
	//////////////////////////
	////////���ʕ�������////////
	//////////////////////////

	HRESULT result;
	//windowsAPI����������
	windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX����������
	directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	//�L�[�{�[�h����������
	input = new Input();
	input->Initialize(windowsAPI);

	//�e�N�X�`���}�l�[�W���[�̏�����
	Texture::Initialize(directX->GetDevice());

	//�X�v���C�g���ʕ��̏�����
	spriteManager = new SpriteManager;
	spriteManager->Initialize(directX, WindowsAPI::winW, WindowsAPI::winH);

	//3D�I�u�W�F�N�g�̏�����
	Object3d::StaticInitialize(directX);

	//�J�����N���X������
	Camera::StaticInitialize(directX->GetDevice());

	////imgui//imgui������
	imguiManager = new ImguiManager();
	imguiManager->Initialize(windowsAPI, directX);

	//�I�[�f�B�I������
	AudioManager::StaticInitialize();

	//////////////////////////
	/////���ʕ������������܂�/////
	//////////////////////////

	//--------------�Q�[�����ϐ�������--------------//

	//�e�N�X�`���f�[�^������
	magnetTextureN = Texture::LoadTexture(L"Resources/red1x1.png");
	magnetTextureS = Texture::LoadTexture(L"Resources/blue1x1.png");
	groundTexture = Texture::LoadTexture(L"Resources/ground.png");
	playerTexture = Texture::LoadTexture(L"Resources/white1x1.png");

	//�J����������
	XMFLOAT3 eye(5, 25, 6);	//���_���W
	XMFLOAT3 target(5, 0, 6);	//�����_���W
	XMFLOAT3 up(0, 1, 0);		//������x�N�g��

	//XMFLOAT3 eye(25, 5, -10);	//���_���W
	//XMFLOAT3 target(10, 5, 0);	//�����_���W
	//XMFLOAT3 up(0, 1, 0);		//������x�N�g��

	//XMFLOAT3 eye(30, 3, 5);	//���_���W
	//XMFLOAT3 target(5, 3, 5);	//�����_���W
	//XMFLOAT3 up(0, 1, 0);		//������x�N�g��

	camera.Initialize(eye, target, up);

	//�}�b�v�ǂݍ���
	map_ = new Map;
	map_->Loding("map/map1.csv");

	//�}�b�v�̍��W�̏�����
	for (int i = 0; i < blockY; i++)
	{
		for (int j = 0; j < blockZ; j++)
		{
			for (int k = 0; k < blockX; k++)
			{
				blockObj[i][j][k].Initialize();
				blockObj[i][j][k].model = Model::CreateModel();
				blockObj[i][j][k].model->textureIndex = groundTexture;
				blockObj[i][j][k].position.x = k * blockSize * blockScale;
				blockObj[i][j][k].position.y = i * blockSize * blockScale;
				blockObj[i][j][k].position.z = j * blockSize * blockScale;
				blockObj[i][j][k].scale = { blockScale,blockScale,blockScale };
				blockObj[i][j][k].Update();
			}
		}
	}

	//�v���C���[������
	player = new Player();
	player->Initialize(playerTexture, magnetTextureN, magnetTextureS, input,map_);
	player->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	//player->obj.position = XMFLOAT3(0,1,0);


	//���΃f�[�^������
	//MagnetData nBlockPos{ XMFLOAT3(3,2,0.9),true };
	//MagnetData sBlockPos{ XMFLOAT3(6,2,0.9), false };
	MagnetData n2BlockPos{ XMFLOAT3(3, 2, 5), true };
	MagnetData s2BlockPos{ XMFLOAT3(6, 2, 5), false };

	//���΃f�[�^��z��ɍ�������
	//magnetDatas.push_back(nBlockPos);
	//magnetDatas.push_back(sBlockPos);
	magnetDatas.push_back(n2BlockPos);
	magnetDatas.push_back(s2BlockPos);

	//���΂̏������Ɛ���
	for (int i = 0; i < magnetDatas.size(); i++) {
		MagnetBlock newBlock{};
		newBlock.Initialize(magnetDatas[i]);
		//�Q�[���Ŏg���悤�̔z��Ɋi�[
		magnetBlocks.push_back(newBlock);
		magnetBlocks[i].obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	}

	//�����蔻�菉����
	colision = new Colision();
	for (int i = 0; i < magnetDatas.size(); i++) {
		colision->Initialize(player, magnetBlocks[i], map_,i);
	}
}

void MyGame::Finalize()
{
	//WindowsAPI�I������
	windowsAPI->Finalize();

	//��ՃV�X�e�����������������Ԃɉ���B���̌�Q�[�����[�v�Ŏg�p�����������
	delete windowsAPI;
	delete input;
	delete directX;
	delete spriteManager;

	//imguiManager->Finalize();
	delete imguiManager;

	//��������Q�[�����[�v�Ŏg�p��������
	//delete sprite;
	//delete skyDome;
	delete map_;
	delete player;
}

void MyGame::Update()
{
	//windows�̃��b�Z�[�W����
	if (windowsAPI->ProcessMessage()) {
		//���[�v�𔲂���
		endRequest = true;
	}
	//imgui�J�n����
	imguiManager->Begin();
//���͌n�X�V
	input->Update();

	//----------------------�Q�[�������[�v�͂�������---------------------//

	//���͌v�Z
	for (int i = 0; i < magnetDatas.size(); i++) {
		colision->UpdateDeta(player, magnetBlocks[i], i);
	}

	colision->Update();

	//���W�̍X�V
	for (int i = 0; i < magnetBlocks.size(); i++) {

		magnetBlocks[i] = colision->magnetBlocks[i];

		magnetBlocks[i].Update();

	}

	player->Update();

	//�J�������W�͎��@�ɒǏ]
	camera.target.x = player->GetPosition().x;
	camera.target.y = player->GetPosition().y;
	camera.target.z = player->GetPosition().z;
	camera.eye = camera.target;
	camera.eye.y += 20.0f;
	camera.eye.z -= 2.5f;

	camera.UpdateMatrix();

	//----------------------�Q�[�������[�v�͂����܂�---------------------//

	//imgui�I��
	imguiManager->End();

}

void MyGame::Draw()
{
	//�`��O����
	directX->BeginDraw();

	//-------�w�i�X�v���C�g�`�揈��-------//
	spriteManager->beginDraw();


	//-------3D�I�u�W�F�N�g�`�揈��-------//
	Object3d::BeginDraw(camera);

	//���@�`��
	player->Draw();

	//���Ε`��
	for (int i = 0; i < magnetBlocks.size(); i++) {
		magnetBlocks[i].Draw(magnetTextureN, magnetTextureS);
	}

	//�}�b�v�̕`��
	for (int i = 0; i < blockY; i++)
	{
		for (int j = 0; j < blockZ; j++)
		{
			for (int k = 0; k < blockX; k++)
			{
				if (map_->map[i][j][k] == 1)
				{
					blockObj[i][j][k].Draw();
				}
			}
		}
	}


	//-------�O�i�X�v���C�g�`�揈��-------//
	spriteManager->beginDraw();


	//�f�o�b�O�`��
	imguiManager->Draw();

	//�`��㏈��
	directX->EndDraw();
}
