{
	ulong* aDestPixelsRow = ((ulong*) GetBits()) + (theY * mWidth) + theX;	

	if ((mHasAlpha) || (mHasTrans) || (theColor != Color::White))
	{
		if (theColor == Color::White)
		{
			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;
				EACH_ROW;

				for (int x = 0; x < theSrcRect.mWidth; x++)
				{
					ulong src = NEXT_SRC_COLOR;
					int a = src >> 24;	
					
					if (a != 0)
					{
						int aDestAlpha = (*aDestPixels >> 24) - a;
						if (aDestAlpha < 0)	aDestAlpha = 0;
						*(aDestPixels++) = aDestAlpha << 24;
					}
					else
						aDestPixels++;
				}

				aDestPixelsRow += mWidth;
				aSrcPixelsRow += theImage->mWidth;
			}
		}
		else
		{			
			int ca = theColor.mAlpha;
			int cr = theColor.mRed;
			int cg = theColor.mGreen;
			int cb = theColor.mBlue;


			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;
					
				EACH_ROW;

				for (int x = 0; x < theSrcRect.mWidth; x++)
				{
					ulong src = NEXT_SRC_COLOR;
					
					int a = ((src >> 24) * ca) / 255;	
						
					if (a != 0)
					{
						int aDestAlpha = (*aDestPixels >> 24) - a;
						if (aDestAlpha < 0)	aDestAlpha = 0;
						*(aDestPixels++) = aDestAlpha << 24;
					}
					else
						aDestPixels++;
				}

				aDestPixelsRow += mWidth;
				aSrcPixelsRow += theImage->mWidth;
			}
			
		}
	}
	else
	{
		uchar* aSrcRLAlphaData = aSrcMemoryImage->GetRLAlphaData();
		uchar* aRLAlphaDataRow = aSrcRLAlphaData + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;

		for (int y = 0; y < theSrcRect.mHeight; y++)
		{
			ulong* aDestPixels = aDestPixelsRow;
			
			EACH_ROW;
			
			uchar* aRLAlphaData = aRLAlphaDataRow;					

			for (int aSpanLeft = theSrcRect.mWidth; aSpanLeft > 0; )
			{
				ulong src = READ_SRC_COLOR;
				uchar rl = *aRLAlphaData;
				
				if (rl > aSpanLeft)
					rl = aSpanLeft;

				int oma = 256 - (src >> 24);

				if (oma == 1) // Fully opaque
				{
					for (int i = 0; i < rl; i++)
						*aDestPixels++ = NEXT_SRC_COLOR;
				}
				else if (oma == 256) // Fully transparent
				{
					aDestPixels += rl;							
					aSrcPtr += rl;
				}						
				else // Partially transparent
				{

					int a = 256 - oma;
				
					aSrcPtr++;
					int aDestAlpha = (*aDestPixels >> 24) - a;
					if (aDestAlpha < 0)	aDestAlpha = 0;
					*(aDestPixels++) = aDestAlpha << 24;

					for (int i = 1; i < rl; i++)
					{
						ulong src = NEXT_SRC_COLOR;
						int a = (src >> 24);
						int aDestAlpha =( *aDestPixels >> 24) - a;
						if (aDestAlpha < 0)	aDestAlpha = 0;
						*(aDestPixels++) = aDestAlpha << 24;
					}
				}
				
				aRLAlphaData += rl;
				aSpanLeft -= rl;
			}

			aDestPixelsRow += mWidth;
			aSrcPixelsRow += theImage->mWidth;
			aRLAlphaDataRow += theImage->mWidth;
		}
	}
}