#include "MyGame.h"
using namespace DirectX;



MyGame::MyGame()
{
}

void MyGame::Initialize()
{

	//���N���X�̏���������
	KEngineFramework::Initialize();

	//--------------�Q�[�����ϐ�������--------------//

	imguiManager = new ImguiManager();
	imguiManager->Initialize(windowsAPI,directX);

	//�e�N�X�`���f�[�^������
	magnetTextureN = Texture::LoadTexture(L"Resources/red1x1.png");
	magnetTextureS = Texture::LoadTexture(L"Resources/blue1x1.png");
	groundTexture = Texture::LoadTexture(L"Resources/ground.png");
	playerTexture = Texture::LoadTexture(L"Resources/white1x1.png");

	//�J����������
	XMFLOAT3 eye(5, 25, 6);	//���_���W
	XMFLOAT3 target(5, 0, 6);	//�����_���W
	XMFLOAT3 up(0, 1, 0);		//������x�N�g��

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
	player->Initialize(playerTexture, magnetTextureN, magnetTextureS, input);
	player->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);

	//���΃f�[�^������
	MagnetData nBlockPos{ XMFLOAT3(3,2,2),true };
	MagnetData sBlockPos{ XMFLOAT3(6,2,2), false };
	MagnetData n2BlockPos{ XMFLOAT3(3, 2, 5), true };
	MagnetData s2BlockPos{ XMFLOAT3(6, 2, 5), false };

	//���΃f�[�^��z��ɍ�������
	magnetDatas.push_back(nBlockPos);
	magnetDatas.push_back(sBlockPos);
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
		colision->Initialize(player, magnetBlocks[i], i);
	}

}

void MyGame::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//��������Q�[�����[�v�Ŏg�p��������
	imguiManager->Finalize();
	delete imguiManager;
	//delete sprite;
	//delete skyDome;
	delete map_;
	delete player;

	//-------------�����܂łɃ��[�v���Ŏg�p�������̂̌㏈��------------//

	//���N���X�̏I������
	KEngineFramework::Finalize();

}

void MyGame::Update()
{
	//���N���X�̍X�V����
	KEngineFramework::Update();

	//----------------------�Q�[�������[�v�͂�������---------------------//

	//imgui��t�J�n����
	imguiManager->Begin();


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

	//imgui��t�I��
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
