<?php

function display()
{
	glClear(GL_COLOR_BUFFER_BIT);//�J���[�E�o�b�t�@���N���A

	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);//�ԐF
		glVertex2f(0.0, 0.5);

		glColor3f(0.0, 1.0, 0.0);//�ΐF
		glVertex2f(-0.5, -0.5);	

		glColor3f(0.0, 0.0, 1.0);//�F
		glVertex2f(0.5, -0.5);
	glEnd();

	glFlush();//���߂̎��s
}

glutInit($argv);

glutInitWindowSize(400,400);
glutInitDisplayMode(GLUT_RGBA); //�f�B�X�v���C���[�h
glutCreateWindow('Draw Triangle');
	
glutDisplayFunc('display');//�R�[���o�b�N�֐��o�^

/* �J���[�E�o�b�t�@�̏����l*/
glClearColor(1.0, 1.0, 1.0, 1.0);

glutMainLoop();

exit (0);
