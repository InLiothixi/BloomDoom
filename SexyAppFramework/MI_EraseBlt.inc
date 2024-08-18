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
					ulong dest = *aDestPixels;
					
					int a = src >> 24;	
					int destA = dest >> 24;
					
					if (a != 0)
					{
						destA -= a;
						if (destA < 0)
							destA = 0;
						*(aDestPixels++) = destA << 24;
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

#ifdef OPTIMIZE_SOFTWARE_DRAWING
			bool performNormalBlit = true;
			if (cr == cg && cg == cb)
			{
				performNormalBlit = false;
				for (int y = 0; y < theSrcRect.mHeight; y++)
				{
					ulong* aDestPixels = aDestPixelsRow;
					
					EACH_ROW;

					for (int x = 0; x < theSrcRect.mWidth; x++)
					{
						ulong src = NEXT_SRC_COLOR;
						ulong dest = *aDestPixels;
						
						int a = ((src >> 24) * ca) / 255;	
						int destA = dest >> 24;

						if (a != 0)
						{
							destA -= a;
							if (destA < 0)
								destA = 0;
							*(aDestPixels++) = destA << 24;
						}
						else
							aDestPixels++;
					}

					aDestPixelsRow += mWidth;
					aSrcPixelsRow += theImage->mWidth;
				}
			}
			if (performNormalBlit)
#endif
			{
				for (int y = 0; y < theSrcRect.mHeight; y++)
				{
					ulong* aDestPixels = aDestPixelsRow;
					
					EACH_ROW;

					for (int x = 0; x < theSrcRect.mWidth; x++)
					{
						ulong src = NEXT_SRC_COLOR;
						ulong dest = *aDestPixels;
						
						int a = ((src >> 24) * ca) / 255;
						int destA = dest >> 24;
						
						if (a != 0)
						{
							destA -= a;
							if (destA < 0)
								destA = 0;
							*(aDestPixels++) = destA << 24;
						}
						else
							aDestPixels++;
					}

					aDestPixelsRow += mWidth;
					aSrcPixelsRow += theImage->mWidth;
				}
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
#ifdef OPTIMIZE_SOFTWARE_DRAWING
					int a = 256 - oma;
				
					aSrcPtr++;
					ulong dest = *aDestPixels;
					int destA = dest >> 24;
					destA -= src >> 24;

					if (destA < 0)
						destA = 0;

					*(aDestPixels++) = destA << 24;

					for (int i = 1; i < rl; i++)
					{
						ulong src = NEXT_SRC_COLOR;
						int a2 = (src >> 24);

						destA -= a2;
						if (destA < 0)
							destA = 0;

						*(aDestPixels++) = destA << 24;
					}
#else
					int a = 256 - oma;
				
					aSrcPtr++;
					ulong dest = *aDestPixels;
					int destA = dest >> 24;
					*(aDestPixels++) = destA << 24;

					for (int i = 1; i < rl; i++)
					{
						ulong src = NEXT_SRC_COLOR;
						int a = src >> 24;

						destA -= a;
						if (destA < 0)
							destA = 0;

						*(aDestPixels++) = destA << 24;
					}
#endif
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