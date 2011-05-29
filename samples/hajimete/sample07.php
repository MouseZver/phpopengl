<?php

function display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	echo 'display callback'.PHP_EOL;
	glFlush();
}

function reshape($width, $height)
{
	printf("(w, h) = (%d,%d)\n",$width,$height);
}

//---------- ���C���֐�------------//
glutInit($argv);

glutInitWindowSize(400,400);
glutInitDisplayMode(GLUT_RGBA); 
glutCreateWindow('CallBack Test');
		
/* �R�[���o�b�N�֐��̓o�^*/
glutDisplayFunc('display');
glutReshapeFunc('reshape');

glClearColor(1.0, 1.0, 1.0, 1.0);

glutMainLoop();

exit (0);
