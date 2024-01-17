#include "Primitive.h"
#include <glut.h>
#include "CMatrix.h"
#include "Maths.h"

// ������`��i3D�j
void Primitive::DrawLine(const CVector& s, const CVector& e, const CColor& color, float lineWidth)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// �A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	// �u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	glLineWidth(lineWidth);
	// ������`��
	glBegin(GL_LINES);
	glVertex3f(s.X(), s.Y(), s.Z());
	glVertex3f(e.X(), e.Y(), e.Z());
	glEnd();
	glLineWidth(1.0f);

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_BLEND);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// ������`��i2D�j
void Primitive::DrawLine2D(const CVector2& s, const CVector2& e, const CColor& color, float lineWidth)
{
}

// ����`��
void Primitive::DrawSphere(const CMatrix& m, float rad, const CColor& color)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	CMatrix sm;
	sm.Scale(rad, rad, rad);
	// �`��s��𔽉f
	glMultMatrixf((sm * m).M());

	// �A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	// �u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float col[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glColor4fv(col);

	const int cut = 32;
	CVector* vertex = new CVector[cut * cut * 4];
	float* s = new float[cut + 1];
	float* c = new float[cut + 1];
	for (int i = 0; i <= cut; ++i)
	{
		s[i] = sinf((float)M_PI * 2 * i / cut);
		c[i] = cosf((float)M_PI * 2 * i / cut);
	}

	int idx = 0;
	for (int i = 0; i < cut; ++i)
	{
		const float& is1 = s[i];
		const float& is2 = s[i + 1];
		const float& ic1 = c[i];
		const float& ic2 = c[i + 1];
		for (int j = 0; j < cut; ++j)
		{
			const float& js1 = s[j];
			const float& js2 = s[j + 1];
			const float& jc1 = c[j];
			const float& jc2 = c[j + 1];

			vertex[idx] = CVector(js1 * is1, ic1, jc1 * is1);
			vertex[idx + 1] = CVector(js2 * is1, ic1, jc2 * is1);
			vertex[idx + 2] = CVector(js2 * is2, ic2, jc2 * is2);
			vertex[idx + 3] = CVector(js1 * is2, ic2, jc1 * is2);
			idx += 4;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_QUADS, 0, cut * cut * 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	delete[] vertex;
	delete[] s;
	delete[] c;

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_BLEND);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// ���C���[�t���[���̋���`��
void Primitive::DrawWireSphere(const CMatrix& m, float rad, const CColor& color)
{
	glPolygonMode(GL_FRONT, GL_LINE);
	DrawSphere(m, rad, color);
	glPolygonMode(GL_FRONT, GL_FILL);
}

// �O�p�`��`��
void Primitive::DrawTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& color)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// �A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	// �u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// �O�p�`��`��
	glBegin(GL_TRIANGLES);
	glVertex3f(v0.X(), v0.Y(), v0.Z());
	glVertex3f(v1.X(), v1.Y(), v1.Z());
	glVertex3f(v2.X(), v2.Y(), v2.Z());
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_BLEND);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// �|���S����`��
void Primitive::DrawQuad(const CMatrix& m, const CVector2& size, const CColor& color)
{
	// �|���S����4���_
	static const CVector v[4] =
	{
		{ -0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f,  0.5f },
		{  0.5f, 0.0f,  0.5f },
		{  0.5f, 0.0f, -0.5f },
	};

	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// �`��s�񔽉f
	glMultMatrixf(m.M());

	// �A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	// �u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// �|���S����`��
	glBegin(GL_QUADS);
	glVertex3f(v[0].X() * size.X(), v[0].Y(), v[0].Z() * size.Y());
	glVertex3f(v[1].X() * size.X(), v[1].Y(), v[1].Z() * size.Y());
	glVertex3f(v[2].X() * size.X(), v[2].Y(), v[2].Z() * size.Y());
	glVertex3f(v[3].X() * size.X(), v[3].Y(), v[3].Z() * size.Y());
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_BLEND);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

void Primitive::DrawSector(const CVector& pos, const CVector& rot, const float start, const float end, const float size, const CColor& color, const int div)
{
	CVector* vertex = new CVector[div * 3];
	float* s = new float[div + 1];
	float* c = new float[div + 1];
	for (int i = 0; i <= div; i++)
	{
		float angle = Math::Lerp(start, end, (float)i / div);
		angle = Math::DegreeToRadian(angle);
		s[i] = sinf(angle);
		c[i] = cosf(angle);
	}

	int idx = 0;
	for (int i = 0; i < div; ++i)
	{
		const float& is1 = s[i];
		const float& is2 = s[i + 1];
		const float& ic1 = c[i];
		const float& ic2 = c[i + 1];
		vertex[idx] = CVector(0.0f, 0.0f, 0.0f);
		vertex[idx + 1] = CVector(is1, 0.0f, ic1);
		vertex[idx + 2] = CVector(is2, 0.0f, ic2);
		idx += 3;
	}

	glPushMatrix();

	CMatrix tm;
	tm.Translate(pos.X(), pos.Y(), pos.Z());
	CMatrix rm =
		CMatrix().RotateZ(rot.Z()) *
		CMatrix().RotateX(rot.X()) *
		CMatrix().RotateY(rot.Y());
	CMatrix sm;
	sm.Scale(size, size, size);
	glMultMatrixf((sm * rm * tm).M());

	glDisable(GL_LIGHTING);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);

	// DIFFUSE�ԐF�ݒ�
	float coldata[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(coldata);
	glDrawArrays(GL_TRIANGLES, 0, div * 3);

	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);

	glPopMatrix();

	delete[] vertex;
	delete[] s;
	delete[] c;
}