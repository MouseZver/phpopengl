<?php

function display()
{
	glClear(GL_COLOR_BUFFER_BIT);//�J���[�E�o�b�t�@��������
	
	glFlush();//���߂̎��s
}


glutInit($argv);

glutInitWindowSize(640,480);
glutInitDisplayMode(GLUT_RGBA); //�f�B�X�v���C�E���[�h
glutCreateWindow('Color Buffer');
		
glutDisplayFunc('display');//�R�[���o�b�N�֐��o�^

/* �J���[�E�o�b�t�@�̏����l*/
glClearColor(1.0, 1.0, 1.0, 1.0);

glutMainLoop();

exit (0);
