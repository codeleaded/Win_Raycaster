//#include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"

int nMapWidth = 16;				// World Dimensions
int nMapHeight = 16;

float fPlayerX = 14.7f;			// Player Start Position
float fPlayerY = 5.09f;
float fPlayerA = 0.0f;			// Player Start Rotation
float fFOV = 3.14159f / 3.0f;	// Field of View
float fDepth = 16.0f;			// Maximum rendering distance
float fSpeed = 5.0f;			// Walking Speed

String map;

void Setup(AlxWindow* w){
    map = String_New();
    String_Append(&map,"#########.......");
    String_Append(&map,"#########.......");
    String_Append(&map,"#...............");
    String_Append(&map,"#.......########");
    String_Append(&map,"#..............#");
    String_Append(&map,"#......##......#");
    String_Append(&map,"#......##......#");
    String_Append(&map,"#..............#");
    String_Append(&map,"###............#");
    String_Append(&map,"##.............#");
    String_Append(&map,"#......####..###");
    String_Append(&map,"#......#.......#");
    String_Append(&map,"#......#.......#");
    String_Append(&map,"#..............#");
    String_Append(&map,"#......#########");
    String_Append(&map,"#..............#");
    String_Append(&map,"################");
}

void Update(AlxWindow* w){
    if (Stroke(ALX_KEY_A).DOWN)
		fPlayerA -= (fSpeed * 0.75f) * w->ElapsedTime;

	if (Stroke(ALX_KEY_D).DOWN)
		fPlayerA += (fSpeed * 0.75f) * w->ElapsedTime;
	
	if (Stroke(ALX_KEY_W).DOWN)
	{
		fPlayerX += sinf(fPlayerA) * fSpeed * w->ElapsedTime;
		fPlayerY += cosf(fPlayerA) * fSpeed * w->ElapsedTime;
		if (String_Get(&map,(int)fPlayerX * nMapWidth + (int)fPlayerY) == '#')
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed * w->ElapsedTime;
			fPlayerY -= cosf(fPlayerA) * fSpeed * w->ElapsedTime;
		}			
	}

	if (Stroke(ALX_KEY_S).DOWN)
	{
		fPlayerX -= sinf(fPlayerA) * fSpeed * w->ElapsedTime;
		fPlayerY -= cosf(fPlayerA) * fSpeed * w->ElapsedTime;
		if (String_Get(&map,(int)fPlayerX * nMapWidth + (int)fPlayerY) == '#')
		{
			fPlayerX += sinf(fPlayerA) * fSpeed * w->ElapsedTime;
			fPlayerY += cosf(fPlayerA) * fSpeed * w->ElapsedTime;
		}
	}

	for (int x = 0; x < GetWidth(); x++)
	{
		// For each column, calculate the projected ray angle into world space
		float fRayAngle = (fPlayerA - fFOV/2.0f) + ((float)x / (float)GetWidth()) * fFOV;

		// Find distance to wall
		float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
		float fDistanceToWall = 0.0f; //                                      resolution

		char bHitWall = 0;		// Set when ray hits wall block
		char bBoundary = 0;		// Set when ray hits boundary between two wall blocks

		float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
		float fEyeY = cosf(fRayAngle);

		while (!bHitWall && fDistanceToWall < fDepth)
		{
			fDistanceToWall += fStepSize;
			int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
			int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
			
			if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
			{
				bHitWall = 1;
				fDistanceToWall = fDepth;
			}
			else
			{
				if (String_Get(&map,(int)nTestX * nMapWidth + (int)nTestY) == '#')
				{
					bHitWall = 1;

					//vector<pair<float, float>> p;
                    //
					//for (int tx = 0; tx < 2; tx++)
					//	for (int ty = 0; ty < 2; ty++)
					//	{
					//		// Angle of corner to eye
					//		float vy = (float)nTestY + ty - fPlayerY;
					//		float vx = (float)nTestX + tx - fPlayerX;
					//		float d = sqrt(vx*vx + vy*vy); 
					//		float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
					//		p.push_back(make_pair(d, dot));
					//	}
                    //
					//
                    //sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first; });
					//
					//float fBound = 0.01;
					//if (acos(p.at(0).second) < fBound) bBoundary = TRUE;
					//if (acos(p.at(1).second) < fBound) bBoundary = TRUE;
					//if (acos(p.at(2).second) < fBound) bBoundary = TRUE;
				}
			}
		}
	
		int nCeiling = (float)(GetHeight()/2.0) - GetHeight() / ((float)fDistanceToWall);
		int nFloor = GetHeight() - nCeiling;

		float fShade = 1.0f / fDistanceToWall;
        fShade = F32_Clamp(fShade,0.0f,1.0f);
		if (bBoundary)		fShade = 0.0f;
		
		for (int y = 0; y < GetHeight(); y++)
		{
			if(y <= nCeiling)
				Draw(x,y,BLACK);
			else if(y > nCeiling && y <= nFloor)
				Draw(x,y,Pixel_toRGBA(fShade,fShade,fShade,1.0f));
			else
			{				
				float b = (y - (float)GetHeight() * 0.5f) / ((float)GetHeight() * 0.5f);
				b = F32_Clamp(b,0.0f,1.0f);
                Draw(x,y,Pixel_toRGBA(b,b,b,1.0f));
			}
		}
	}

	for (int nx = 0; nx < nMapWidth; nx++)
		for (int ny = 0; ny < nMapWidth; ny++)
		{
            if(String_Get(&map,(int)nx * nMapWidth + (int)ny)=='#') RenderRect(nx * 10.0f,(ny+1) * 10.0f,10.0f,10.0f,BLACK);
			else                                                	RenderRect(nx * 10.0f,(ny+1) * 10.0f,10.0f,10.0f,WHITE);
		}
    RenderRect((fPlayerX+1) * 10.0f,(fPlayerY) * 10.0f,5.0f,5.0f,GRAY);
}

void Delete(AlxWindow* w){
    String_Free(&map);
}

int main(){
    if(Create("Raycasting World",1900,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}