#include "Primitive.h"
#include "glut.h"
#include "CMatrix.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "Math.h"

void Primitive::DrawLine(const CVector& s, const CVector& e, const CVector4& color, const float width)
{
	glPushMatrix();

	glDisable(GL_LIGHTING);

	//ägéUåıÇÃê›íË
	float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(width);

	glColor4f(color.X(), color.Y(), color.Z(), color.W());
	glBegin(GL_LINES);
	glVertex3f(s.X(), s.Y(), s.Z());
	glVertex3f(e.X(), e.Y(), e.Z());
	glEnd();

	glLineWidth(1.0f);

	glDisable(GL_BLEND);

	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void Primitive::DrawSector(const CVector& pos, const CVector& rot, const float start, const float end, const float size, const CVector4& color, const int div)
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

	glColor4f(color.X(), color.Y(), color.Z(), color.W());
	glDrawArrays(GL_TRIANGLES, 0, div * 3);

	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);

	glPopMatrix();

	delete[] vertex;
	delete[] s;
	delete[] c;
}

void Primitive::DrawCube(const CVector& pos, const CVector& size, const CVector4& color)
{
}

void Primitive::DrawSphere(const CVector& pos, const float rad, const CVector4& color, const int div)
{
	CVector* vertex = new CVector[div * div * 4];
	float* s = new float[div + 1];
	float* c = new float[div + 1];
	for (int i = 0; i <= div; ++i)
	{
		s[i] = sinf((float)M_PI * 2 * i / div);
		c[i] = cosf((float)M_PI * 2 * i / div);
	}

	int idx = 0;
	for (int i = 0; i < div; i++)
	{
		const float& is1 = s[i];
		const float& is2 = s[i + 1];
		const float& ic1 = c[i];
		const float& ic2 = c[i + 1];
		for (int j = 0; j < div; j++)
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

	glPushMatrix();

	glDisable(GL_LIGHTING);

	CMatrix tm;
	tm.Translate(pos.X(), pos.Y(), pos.Z());
	CMatrix sm;
	sm.Scale(rad * 2.0f, rad * 2.0f, rad * 2.0f);
	glMultMatrixf((sm * tm).M());

	glColor3f(color.X(), color.Y(), color.Z());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, vertex);

	glDrawArrays(GL_QUADS, 0, div * div * 4);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);

	glPopMatrix();

	delete[] vertex;
	delete[] s;
	delete[] c;
}
