/***************************************************************************************************************/
/*Calculates the dispersion relation and the transmission through a superlattice for LA phonons                */
/***************************************************************************************************************/


#include<stdio.h>
#include<math.h>
#include<stdlib.h>
FILE *fp1, *fp2, *fopen();
main()
{
	int N;         /*number of superlattice periods*/
	int j,k,m,p; /*counters*/
	int MatType;   /*material type (1=AlGaAs/AlAs, 2=user defined materials*/
	int SubDet;    /*Substrate and detector are GaAs (1=yes, 2=no)*/

	char opfile1[20]; /*the file in which output will be written*/
	char opfile2[20]; /*the file in which output will be written*/

	float i,s,c;       /*counter for wavelength*/
	
	float x;	       /*Al fraction in the AlGaAs well (layer A)*/
	float y;		   /*Al fraction in the AlGaAs barrier (layer B)*/
	float Da;      /*thickness of layer A*/      
	float Db;      /*thickness of layer B*/      
	float Ra;	   /*density of layer A*/
	float Rb;	   /*density of layer B*/
	float Rs;	   /*density of substrate layer*/
	float Rd;	   /*density of detector layer*/
	float Del;
	float coeff;
	
	float D;		/*superlattice period*/      
	
	float Z[150];	/*position along growth direction of differnt layers*/
	float V[150];   /*velocities in different layers */
	float U[150];   /*elastic constants of different layers*/
	float K[150];   /*wavevector */

	float a_cos[10];  /*real compenents in matrix*/
	float a_sin[10];  /*imaginary components in matrix*/
	float b_cos[10];
	float b_sin[10];
	float c_cos[10];
	float c_sin[10];
	float d_cos[10];
	float d_sin[10];
	
	float temp_cos[50];
	float temp_sin[50];
	
	float ref_amp_cos;   /*real part of the reflected wave amplitude*/
	float ref_amp_sin;   /*imaginary part of the reflected wave amplitude*/
	float trans_amp_cos; /*real part of the transmitted wave amplitude*/
	float trans_amp_sin; /*imaginary part of the transmitted wave amplitude*/
	float trans_rate;    /*transmission rate*/
	
	
	/*************************************Name, create and open the output files*********************************/

	printf("Enter the output file name for the transmission data:");
	scanf("%s", opfile1);
	printf("Enter the output file name for the dispersion data:");
	scanf("%s", opfile2);

	fp1 = fopen(opfile1, "w");
	fp2 = fopen(opfile2, "w");

	/************************************************************************************************************/
	

	/*************************************Input the calculation parameters***************************************/

	/*input the superlattice conformation*/
	printf("\nEnter the number of periods in the superlattice:");
	scanf("%d", &N);
	printf("\nEnter the thickness of layer A (well) in Angstroms:");
	scanf("%f", &Da);
	printf("\nEnter the thickness of layer B (barrier) in Angstroms:");
	scanf("%f", &Db);

	/*change the unit system for the calculation*/
	Da=Da*pow(10,-10); /*thickness*/
	Db=Db*pow(10,-10);
	D=Da+Db;
			
	/*input the material properties*/
	printf("\nEnter the type of materials in the superlattice (enter 1 for Al(x)Ga(1-x)As/Al(y)Ga(1-y)As or 2 for user defined materials):");
	scanf("%d", &MatType);

	if(MatType==1)  //AlGaAs/AlGaAs
	{
	
		/*deal with the substrate and detector layers*/
		printf("\nFor AlGaAs/AlGaAs superlattices the substrate and the detector layer are assumed to be GaAs. Is this ok? Enter 1 for yes or 2 for no:");
		scanf("%d", &SubDet);

		if(SubDet==2)  //if the substrate and detector layers are not GaAs
		{

			/*input the material parameters*/
			printf("\nEnter the velocity of phonons in the substrate layer (in m/s):");
			scanf("%f", &V[0]);
			printf("\nEnter the velocity of phonons in the detector layer (in m/s):");
			scanf("%f", &V[3]);
			printf("\nEnter the density of the substrate layer (in units of Kg/m3):");
			scanf("%f", &Rs);
			printf("\nEnter the density of the detector layer (in units of Kg/m3):");
			scanf("%f", &Rd);

			U[0]=(Rs*pow(V[0],2));
			U[2*N+1]=(Rd*pow(V[3],2));

		}
		else  //if the substrate and detector layers are GaAs
		{

			/*set the velocity and elastic constant*/
			V[0]=4730;
			V[3]=4730;
			U[0]=11.90;
			U[2*N+1]=11.90;

		}

		/*input the Al fraction and calculate the properties*/
		printf("\nEnter the Al fraction, x, in layer A (well) (Al(x)Ga(1-x)As):");
		scanf("%f", &x);
		printf("\nEnter the Al fraction, y, in layer B (barrier) (Al(y)Ga(1-y)As):");
		scanf("%f", &y);
		
		V[1]= (4.73 + (0.68*x) + (0.24*pow(x,2)))*pow(10,3);  /*sound velocity*/
		V[2]=(4.73 + (0.68*y) + (0.24*pow(y,2)))*pow(10,3);

		Ra=(5.32 - (1.56*x))*pow(10,3);  /*density*/
		Rb=(5.32 - (1.56*y))*pow(10,3);

		U[1]= (11.88 + (0.14*x));  /*elastic constant*/
		U[2]= (11.88 + (0.14*y));

	
	}
	else  //user defined materials
	{

		/*sound velocity*/
		printf("\nEnter the velocity of phonons in the substrate layer (in m/s):");
		scanf("%f", &V[0]);
		printf("\nEnter the velocity of phonons in layer A (well) (in m/s):");
		scanf("%f", &V[1]);
		printf("\nEnter the velocity of phonons in layer B (barrier) (in m/s):");
		scanf("%f", &V[2]);
		printf("\nEnter the velocity of phonons in the detector layer (in m/s):");
		scanf("%f", &V[3]);
		//V[1]= 4.73*pow(10,3);
		//V[2]= 5.65*pow(10,3);
		
		/*density*/
		printf("\nEnter the density of the substrate layer (in units of Kg/m3):");
		scanf("%f", &Rs);
		printf("\nEnter the density of layer A (well) (in units of Kg/m3):");
		scanf("%f", &Ra);
		printf("\nEnter the density of layer B (barrier) (in units of Kg/m3):");
		scanf("%f", &Rb);
		printf("\nEnter the density of the detector layer (in units of Kg/m3):");
		scanf("%f", &Rd);
		//Ra = 5.32*pow(10,3);
		//Rb = 3.76*pow(10,3);

		/*elastic constant*/
		U[0]=(Rs*pow(V[0],2));
		U[1]=(Ra*pow(V[1],2));
		U[2]=(Rb*pow(V[2],2));
		U[2*N+1]=(Rd*pow(V[3],2));
		//U[1] = 14.032;
		//U[2] = 14.24;

	}
	
	/**********************************************************************************************************/
	
	/*tell user to wait*/
	printf("Processing.....wait");

	/***********************************Initalise the superlattice**********************************************/

	/*initialise the distance along the Z direction*/
	Z[0]=0;
	for(p=1; p<2*N;p=p+2)
	{
		
		Z[p]=Z[p-1]+Da; 
		Z[p+1]=Z[p-1]+D;
		/*printf("%f, %f \n",Z[p],Z[p+1]);*/
		
	}

	/*initialize the elastic constant values for different layers*/	
	for(m=3;m<2*N;m=m+2)
	{
		U[m]=U[1];
		U[m+1]=U[2];
		/*printf("%f, %f \n",U[m],U[m+1]);*/
	}

	/***********************************************************************************************************/
	
	
	/***************************************Start the calculation***********************************************/
	
	/*calculate the coefficient for the dispersion*/
	Del = (Ra*V[1])/(Rb*V[2]);
	coeff = (1+ pow(Del,2))/(2*Del);

	for (i=.01*pow(10,12); i<=10*pow(10,12); i=i+.01*pow(10,11))  //loop for calculating at different frequencies
	{
		/*calculate and output the dispersion relation*/
		s = cos(Da*i/V[1])*cos(Db*i/V[2])-coeff*sin(Da*i/V[1])*sin(Db*i/V[2]);
		c= acos(s);
		if (c > 0)
		{
			fprintf(fp2,"%f, %f \n",c/3.14 ,i*pow(10,-9)/6.28 );
		}

		/*initialize the wavevector in the different layers*/
		K[0]=i/V[0];
		K[1]=i/V[1];
		K[2]=i/V[2];
		K[2*N+1]=i/V[3];

		for(m=3;m<2*N;m=m+2)
		{
			K[m]=K[1];
			K[m+1]=K[2];
			//printf("%f,, %f \n",K[m],K[m+1]);
		}		


		/*initializes matrix d as a unit matrix (the initial conditions)*/
		d_cos[1]=d_cos[4]=1;
		d_cos[2]=d_cos[3]=0;
		d_sin[1]=d_sin[4]=0;
		d_sin[3]=d_sin[2]=0;	
		
		for(k=0;k<=2*N;k++)  //loop for calculating in each layer
		{
		
			a_cos[1]= cos(K[k]*Z[k]); /*matrix a represents h in medium I*/
			a_cos[2]= a_cos[1];
			a_cos[3]= U[k]*K[k]*cos(K[k]*Z[k]);
			a_cos[4]= -a_cos[3];
			/*printf("%f,%f,%f,%f\n", a_cos[1],a_cos[2], a_cos[3],a_cos[4]);*/
	
			a_sin[1]= sin(K[k]*Z[k]);
			a_sin[2]= -a_sin[1];
			a_sin[3]= U[k]*K[k]*sin(K[k]*Z[k]);
			a_sin[4]= a_sin[3];
			/*printf("%f,%f,%f,,%f\n", a_sin[1],a_sin[2], a_sin[3], a_sin[4]);*/
			
			b_cos[1]= cos(K[k+1]*Z[k])/2; /*matrix b represents h-inv in medium II*/
			b_cos[2]= (cos(K[k+1]*Z[k]))/(2*U[k+1]*K[k+1]);
			b_cos[3]= b_cos[1];
			b_cos[4]= -b_cos[2];
			/*printf("%f,%f,%f,%f\n", b_cos[1],b_cos[2], b_cos[3], b_cos[4]);*/
			
			b_sin[1]= -sin(K[k+1]*Z[k])/2;
			b_sin[2]= -(sin(K[k+1]*Z[k]))/(2*U[k+1]*K[k+1]);
			b_sin[3]= -b_sin[1];
			b_sin[4]= b_sin[2];
			/*printf("%f,%f,%f,%f\n", b_sin[1],b_sin[2], b_sin[3], b_sin[4]);*/
			
			for(p=1,j=1;j<5; j=j+2)
			{
				c_cos[j]= b_cos[j]*a_cos[p] - b_sin[j]*a_sin[p] + b_cos[j+1]*a_cos[p+2]- b_sin[j+1]*a_sin[p+2];
				c_sin[j]= b_sin[j]*a_cos[p] + b_cos[j]*a_sin[p] + b_cos[j+1]*a_sin[p+2]+ b_sin[j+1]*a_cos[p+2];
			
				c_cos[j+1]= b_cos[j]*a_cos[p+1] - b_sin[j]*a_sin[p+1] + b_cos[j+1]*a_cos[p+3]- b_sin[j+1]*a_sin[p+3];			
				c_sin[j+1]= b_sin[j]*a_cos[p+1] + b_cos[j]*a_sin[p+1] + b_cos[j+1]*a_sin[p+3]+ b_sin[j+1]*a_cos[p+3];
			
			}
			/*printf("%f,%f,%f,%f\n", c_cos[1],c_cos[2], c_cos[3], c_cos[4]);*/
			/*printf("%f,%f,%f,%f\n", c_sin[1],c_sin[2], c_sin[3], c_sin[4]);*/
			
			for(p=1,j=1;j<5; j=j+2)
			{
				temp_cos[j]= c_cos[j]*d_cos[p] - c_sin[j]*d_sin[p] + c_cos[j+1]*d_cos[p+2]- c_sin[j+1]*d_sin[p+2];
				temp_sin[j]= c_sin[j]*d_cos[p] + c_cos[j]*d_sin[p] + c_cos[j+1]*d_sin[p+2]+ c_sin[j+1]*d_cos[p+2];
			/*printf("%f,",temp_cos[j]);printf("%f\n",temp_sin[j]);*/
				temp_cos[j+1]= c_cos[j]*d_cos[p+1] - c_sin[j]*d_sin[p+1] + c_cos[j+1]*d_cos[p+3]- c_sin[j+1]*d_sin[p+3];			
				temp_sin[j+1]= c_sin[j]*d_cos[p+1] + c_cos[j]*d_sin[p+1] + c_cos[j+1]*d_sin[p+3]+ c_sin[j+1]*d_cos[p+3];
			/*printf("%f",temp_cos[j+1]);printf("%f\n",temp_sin[j+1]);*/
			}
			for(m=1;m<=4;m++)
			{
				d_cos[m]=temp_cos[m];
				d_sin[m]=temp_sin[m];
				/*printf("%f,%f\n",d_cos[m],d_sin[m]);*/
			}
			/*printf("%f,%f,%f,%f\n", d_cos[1],d_cos[2], d_cos[3], d_cos[4]);*/
			/*printf("%f,%f,%f,%f\n", d_sin[1],d_sin[2], d_sin[3], d_sin[4]);*/
	
		}
		/*printf("%d\n",i);*/
		for(m=1;m<=4;m++)
		/*printf("%f,%f\n",d_cos[m],d_sin[m]);*/
		
		ref_amp_cos=-((d_cos[3]*d_cos[4] + d_sin[3]*d_sin[4])/(d_sin[4]*d_sin[4] + d_cos[4]*d_cos[4]));
		ref_amp_sin=-((d_sin[3]*d_cos[4] - d_cos[3]*d_sin[4])/(d_sin[4]*d_sin[4] + d_cos[4]*d_cos[4]));
		trans_amp_cos = d_cos[1] + d_cos[2]*ref_amp_cos - d_sin[2]*ref_amp_sin;
		trans_amp_sin = d_sin[1] + d_sin[2]*ref_amp_cos + d_cos[2]*ref_amp_sin;
		/*printf("%f,%f,%f,%f\n",ref_amp_cos, ref_amp_sin, trans_amp_cos, trans_amp_sin);*/
		trans_rate = trans_amp_cos*trans_amp_cos + trans_amp_sin*trans_amp_sin;
		fprintf(fp1,"%f %f\n",trans_rate, i*pow(10,-9)/6.28 );
							  
	
	}
	fflush(fp1);
	fclose(fp1);
	
}
