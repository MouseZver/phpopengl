<?php

//--------- �R�[���o�b�N�֐�--------------//
function display()
{
	glClear(GL_COLOR_BUFFER_BIT);//�J���[�E�o�b�t�@���N���A
	glColor3f(1.0, 0.0, 0.0);

	glutWireTeapot(1.0);//�e�B�[�|�b�g��`��

	glFlush();//���߂̎��s
}

function reshape($w, $h)
{
	glViewport(0, 0, $w, $h);  //�r���[�|�[�g�̐ݒ�

	glMatrixMode(GL_PROJECTION);//�ˉe�s��̐ݒ�
	glLoadIdentity();
	gluPerspective(30.0, $w / $h, 1.0, 100.0); 

	glMatrixMode(GL_MODELVIEW);//���f���r���[�s��̐ݒ�
	glLoadIdentity();
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


//---------- ���C���֐�-------------//
glutInit($argv);

glutInitWindowSize(640,480);
glutInitDisplayMode(GLUT_RGBA); //�f�B�X�v���C�E���[�h
glutCreateWindow('Hello 3D Graphics');
		
glutDisplayFunc('display');//�R�[���o�b�N�֐��o�^
glutReshapeFunc('reshape');

/* �J���[�E�o�b�t�@�̏����l*/
glClearColor(1.0, 1.0, 1.0, 1.0);

glutMainLoop();

exit (0);

