#include <stdio.h>
#include <stdlib.h>

int main() {
	int min=0, max=0, total_datos=0, veces=0, t_min=0, t_max=0;
	float tiempo=0, t_tiempo=0;
	char s_tiempo[10];
	printf("Ingrese max carga: ");
	scanf("%d", &max);
	printf("Ingrese min carga: ");
	scanf("%d", &min);

	FILE *fp;
	fp = fopen ("logs", "r");
	while(!feof(fp)) {
		fscanf(fp, "%d %d %s \n",&t_max,&t_min, &s_tiempo);
		t_tiempo=atof(s_tiempo);
		if (t_max==max && t_min==min) {
			veces++;
			tiempo+=t_tiempo;
		}
		total_datos++;
	}
	printf("%f  %f\n",100*(float)veces/total_datos, tiempo/veces);
	fclose(fp);
	return 0;
}