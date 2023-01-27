#pragma once
class KEngineFramework
{
	//KEngineFramework();

	virtual ~KEngineFramework() = default;

	//������
	virtual void Initialize();
	
	//�I��
	virtual void Finalize();

	//���t���[���X�V
	virtual void Update();

	//�`��
	virtual void Draw() = 0;

	//�I���`�F�b�N
	virtual bool IsEndRequest() { return endRequest; }

	//�����̑S���s
	void Run();

	bool endRequest = false;
};

