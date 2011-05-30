<?php

//�O�p�`�̕`��
function drawTriangle()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0, 0.5, 0.0);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glEnd();
}

//-------- XYZ���̕`�� -------------//
function drawXYZ()
{
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);//X��:��
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(200.0, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);//Y��:��
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 200.0, 0.0);

		glColor3f(0.0, 0.0, 1.0);//Z��:��
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 200.0);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);//�F�̃��Z�b�g
}


//-------- �e��R�[���o�b�N�֐�------------//
function display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glPushMatrix();
		glTranslatef(1.0, 0.0, 0.0);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		glColor3f(1.0, 1.0, 0.0);//���F
		drawTriangle();
	glPopMatrix();

	glPushMatrix();
		glRotatef(45.0, 0.0, 1.0, 0.0);
		glTranslatef(1.0, 0.0, 0.0);
		glColor3f(0.7, 0.7, 0.7);//�D�F
		drawTriangle();
	glPopMatrix();

	drawXYZ();//XYZ���̕`��
	glFinish();
}

function reshape($w, $h)
{
	glViewport(0,0,$w,$h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0,$w/$h, 0.1, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 2.5, 4,0, 0.0, 0.0, 0.0, 1.0, 0.0);	
}

//-------- ���̑��e��ݒ�-----------------//
function otherInit()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
}

//--------- ���C���֐�------------//
glutInit($argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
glutInitWindowSize(640,480);
glutCreateWindow('Depth and Affine');
	
//�R�[���o�b�N�֐��o�^
glutDisplayFunc('display');
glutReshapeFunc('reshape');

otherInit();//���̑��ݒ�

glutMainLoop();

exit (0);


