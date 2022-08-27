#include "stdafx.h"
#include "collision.h"

namespace Neptune_UTIL
{
	bool isCollisionReaction(const RECT& rcHold, RECT& rcMove)
	{
		//��Ʈ���� �浹������ �ȿ� �����Ǵ� ��Ʈ 
		RECT rcInter;

		//�浹���� �ʾҴٸ� ������ ���ʿ��ϴ�!
		if (!IntersectRect(&rcInter, &rcHold, &rcMove)) return false;

		int interSectorW = rcInter.right - rcInter.left;
		int interSectorH = rcInter.bottom - rcInter.top;
		//�������� �ε����ٸ�~~~
		if (interSectorW > interSectorH)
		{
			//������ �ھ�����
			if (rcInter.top == rcHold.top)
			{
				OffsetRect(&rcMove, 0, -interSectorH);
			}
			else if (rcInter.bottom == rcHold.bottom)//�Ʒ����� �ε����ٸ�
			{
				OffsetRect(&rcMove, 0, interSectorH);
			}
		}
		else
		{
			//�����ʿ��� ���� ������
			if (rcInter.left == rcHold.left)
			{
				OffsetRect(&rcMove, -interSectorW, 0);
			}
			else if (rcInter.right == rcHold.right)
			{
				OffsetRect(&rcMove, interSectorW, 0);
			}
		}
	}

}