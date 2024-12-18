#include<stdio.h>
#include<string.h>

/*
Transforms a trigram(given as an array of 3 chars) into a unified form
ie transform(123)=transform(312)=transform(231)=123

How it works:
-first it finds the smallest value glyph in the trigram(min)
-then an occurrence of this minimal glyph that is preceeded by a different glyph
	(this is essentially to "group" all of the minimal glyphs at the start of the representation eg.: transform(121)->112
-now "rotate" the trigram such that the glyph found in the previous step is at the start(index 0) and the order of the glyphs is preseved

example for a few values:

321:
-minimal glyph is '1'
-starting_point is 2
- rotation: 321 -> 132

132:
-minimal glyph is '1'
-starting point is 0
-rotation: 132 -> 132
(Notice how this is a fixed point of the function, this means that we can find all of the valid symbols by looping through all possible trigrams
 and selecting the ones whose transformed is themself)
 
232
-minimal glyph is '2'
-starting point is 2(since the '2' at 0 is preceeded by a '2')
-rotation: 232 -> 223

322
-minimal glyph is '2'
-starting point is 1
-rotation: 322 -> 223
*/
void transform(char trigram[3])
{
	char min = trigram[0];
	for(int i = 1; i < 3; ++i)//find minimal glyph value
	{
		if(trigram[i]<min)min=trigram[i];
	}
	int starting_point = 0;
	for(int i = 0; i < 3; ++i) //Find a glyph whose value is minimal and who is preceded by a glyph not equal to it(ie greater since it has the minimal value)
	{
		if(trigram[i]==min && trigram[(i-1)%3]!=min)starting_point=i;
	}
	//The rotation
	char temp[3] = {trigram[starting_point],trigram[(starting_point+1)%3], trigram[(starting_point+2)%3]};
	memcpy(trigram,temp,3);
	
	return;
}

int process(FILE*source)
{
	int frequencies[5][5][5]={0};
	int c = fgetc(source);
	char message_name[32] = {0};
	int index = 0;	
	while(c!=',')//Read the message name
	{
		message_name[index++]=c;
		c=fgetc(source);
		if(c==EOF)return 1;
	}
	char buffer[3];
	index=0;
	while(c!='\n')//Scan through the glyphs
	{
		c=fgetc(source);
		if(c==EOF){printf("EOF?!\n");break;}
		buffer[index++]=c;
		if(index==3)//After an entire trigram has been read
		{
			index=0;
			transform(buffer);//Transform the read trigram into the proper form(See transform for more information)
			frequencies[buffer[0]-'0'][buffer[1]-'0'][buffer[2]-'0']+=1;//Add the occurrence to the frequency table
		}
	}
	printf("%s\n",message_name);
	int magic_index = 0;
	for(int i = 0; i < 5;++i){		//Loop through all trigrams in the table and print their frequency
		for(int j = 0; j < 5;++j){
			for(int k = 0; k < 5;++k)
			{
				char trigram[4] = {i+'0',j+'0',k+'0',0};
				char copy[4];
				strcpy(copy,trigram);
				transform(copy);
				if(strcmp(copy,trigram)==0)//Only print fixed points of transform
					printf("%1d%1d%1d\t%d\t%d\n",i,j,k,magic_index++,frequencies[i][j][k]);
	}}}
	return 0;
}

int main()
{
	
	/*
	//This lists all the rotation agnostic trigrams
	char temp[4] = {0};
	char temp2[4]= {0};
	for(int i = 0; i < 5*5*5;++i)
	{
		temp[0] = (i/(5*5))%5+'0';
		temp[1] = (i/5)%5+'0';
		temp[2] = i%5+'0';
		strcpy(temp2,temp);
		transform(temp2);
		if(strcmp(temp,temp2)==0) //if you remove this conditional it'll instead list all trigrams and their transformed values
		{
			printf("trigram:%s transformed:%s\n",temp,temp2);
		}
	}*/
	FILE*thing=fopen("raw.txt","r");
	while(process(thing)==0);
	fclose(thing);
	return 0;
}