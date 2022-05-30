#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

const float theta_spacing = 0.07;
const float phi_spacing = 0.02;
const float two_pi_rad = 6.28;


const float R2 = 2;
const float R1 = 1;

const float K2 = 5;
//const float K1 = screen_width*K2*3/(8*(R1+R2));
const float K1 = 150;

void animate_donut(float A,float B)
{
	float theta,phi;
	int k;
	float z_buffer[1760];
	char output[1760];
	printf("\x1b[23"); //clears screen
	for(;;)
	{
		float cosA = cos(A);
		float cosB = cos(B);
		float sinA = sin(A);
		float sinB = sin(B);
		//filling the buffer with values
		memset(output,' ',1760 ); // no random characters will appear in the output now
		memset(z_buffer,0,7040);
		for(theta=0; theta<two_pi_rad; theta+=theta_spacing)
		{
			float costheta = cos(theta);
			float sintheta = sin(theta);
			for(phi=0; phi<two_pi_rad; phi+=phi_spacing)
			{
				float cosphi = cos(phi);
				float sinphi = sin(phi);

				float h = costheta + 2; // equivalent to R2 + R1 * costheta
				float t = sinphi*h*cosA-sintheta*sinA;

				// calculating x,y coordinate
				float x = cosphi*h*cosB-t*sinB;
				float y = cosphi*h*sinB+t*cosB;
				
				// calculate 1/z
				float one_over_z = 1/(sinphi*h*sinA+sintheta*cosA+5);
				
				// x and y projection.
      			int xp = (int) (40 + 30*one_over_z*x);
      			int yp = (int) (12 + 15*one_over_z*y);
      
     			// calculate luminance.
      			float L = cosphi*costheta*sinB - cosA*costheta*sinphi -sinA*sintheta + cosB*(cosA*sintheta - costheta*sinA*sinphi);
      			int luminos_intensity = 8 * L;
      			int o = xp + 80 * yp;
      			if (22 > yp && yp > 0 && xp > 0 && 79 > xp && one_over_z > z_buffer[o]) 
      			{
         			z_buffer[o] = one_over_z;
          			output[o] = ".,-~:;=!*#$@"[luminos_intensity > 0 ? luminos_intensity : 0];
        		}
      		}
		}
		printf("\x1b[H"); //opens in new shell after clearing terminal
		for(k=0;k<1761;k++)
		{
			putchar(k % 80 ? output[k] : 10);
            A += 0.00004;
            B += 0.00002;
		}
		usleep(3000);
	}
}

int main()
{
	const float transform_x_axes = 0;
	const float transform_z_axes = 0;
	animate_donut(transform_x_axes,transform_z_axes);
	return 0;
}