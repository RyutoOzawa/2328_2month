#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"
#include"ShareData.h"

using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�e�N�X�`���f�[�^������
	magnetTextureN = Texture::LoadTexture(L"Resources/red1x1.png");
	magnetTextureS = Texture::LoadTexture(L"Resources/blue1x1.png");
	groundTexture = Texture::LoadTexture(L"Resources/ground.png");
	playerTexture = Texture::LoadTexture(L"Resources/white1x1.png");
	backGroundTexture = Texture::LoadTexture(L"Resources/dummyPlayGame.png");
	clearTexture = Texture::LoadTexture(L"Resources/clear.png");

	backGroundSprite = new Sprite();
	backGroundSprite->Initialize(backGroundTexture);

	//�J����������
	XMFLOAT3 eye(5, 25, 6);	//���_���W
	XMFLOAT3 target(5, 0, 6);	//�����_���W
	XMFLOAT3 up(0, 1, 0);		//������x�N�g��

	camera.Initialize(eye, target, up);

	//�}�b�v�ǂݍ���
	map_ = new Map;

	//�X�e�[�W�I�����ɕύX���ꂽ������œǂݍ���
	SetStage(ShareData::stageNumber);
	map_->Loding(stageStr.c_str());

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

				if (map_->map[i][j][k] == 2) {

				}
			}
		}
	}

	//�v���C���[������
	player = new Player();
	player->Initialize(playerTexture, magnetTextureN, magnetTextureS, input, map_, goal);
	for (int i = 0; i < blockY; i++)
	{
		for (int j = 0; j < blockZ; j++)
		{
			for (int k = 0; k < blockX; k++)
			{
				if (map_->map[i][j][k] == 4)
				{
					player->SetPosition({ k * blockSize * blockScale,i * blockSize * blockScale,j * blockSize * blockScale });
				}
			}
		}
	}

	player->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);

	goal = new Goal;
	goal->Initialize(input, goalTexture, XMFLOAT3(9, 4, 9));
	goal->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	goal->obj.Update();

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
		colision->Initialize(player, magnetBlocks[i], map_,i);
	}

}

void GamePlayScene::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//delete sprite;
	//delete skyDome;
	delete map_;
	delete player;

	delete backGroundSprite;
	//-------------�����܂łɃ��[�v���Ŏg�p�������̂̌㏈��------------//



}

void GamePlayScene::Update()
{

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

	//goal->isGoal = player->GetIsGoal();

	goal->Update();

	//�J�������W�͎��@�ɒǏ]
	camera.target.x = player->GetPosition().x;
	camera.target.y = player->GetPosition().y;
	camera.target.z = player->GetPosition().z;
	camera.eye = camera.target;
	camera.eye.y += 20.0f;
	camera.eye.z -= 2.5f;

	camera.UpdateMatrix();



	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GamePlayScene::Draw()
{
	//-------�w�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	backGroundSprite->Draw();

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

	goal->Draw();

	//-------�O�i�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();


	if (goal->isGoal) {
		goalSprite.Draw();
	}

}

void GamePlayScene::SetStage(int stageNumber)
{
	switch (stageNumber)
	{
	case Sample1:
		stageStr = "map/map1.csv";
		break;
	case Sample2:
		stageStr = "map/map2.csv";
		break;
	case Sample3:
		stageStr = "map/map3.csv";
		break;
	case Sample4:
		stageStr = "map/map4.csv";
		break;
	case tutorial1:
		stageStr = "map/Tuto1.csv";
		break;
	default:
		break;
	}

}