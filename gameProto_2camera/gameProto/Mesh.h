// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	bool Load(const std::string& fileName, class Renderer* renderer);   	// ���b�V���̃��[�h�E�A�����[�h
	void Unload();                                                         	

	class VertexArray* GetVertexArray() { return mVertexArray; }            // ���b�V���̒��_�z��̎擾
	class Texture* GetTexture(size_t index);                                // �w�肳�ꂽ�C���f�b�N�X����e�N�X�`���̎擾
	const std::string& GetShaderName() const { return mShaderName; }        // �V�F�[�_�[���̎擾

	float GetRadius() const { return mRadius; }                             // �o�E���f�B���O�X�t�B�A�̔��a���擾

private:
	std::vector<class Texture*> mTextures;                                 	// ���b�V���̃e�N�X�`��
	class VertexArray* mVertexArray;                                        // ���b�V���̒��_�z��

	std::string mShaderName;	                                            // ���b�V���w��̃V�F�[�_�[
	float mRadius;	                                                        // �o�E���f�B���O�X�t�B�A�̔��a
	float mSpecPower;	                                                    // �\�ʂ̃X�y�L�����[�l

};