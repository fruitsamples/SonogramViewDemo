/*	Copyright © 2007 Apple Inc. All Rights Reserved.
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by 
			Apple Inc. ("Apple") in consideration of your agreement to the
			following terms, and your use, installation, modification or
			redistribution of this Apple software constitutes acceptance of these
			terms.  If you do not agree with these terms, please do not use,
			install, modify or redistribute this Apple software.
			
			In consideration of your agreement to abide by the following terms, and
			subject to these terms, Apple grants you a personal, non-exclusive
			license, under Apple's copyrights in this original Apple software (the
			"Apple Software"), to use, reproduce, modify and redistribute the Apple
			Software, with or without modifications, in source and/or binary forms;
			provided that if you redistribute the Apple Software in its entirety and
			without modifications, you must retain this notice and the following
			text and disclaimers in all such redistributions of the Apple Software. 
			Neither the name, trademarks, service marks or logos of Apple Inc. 
			may be used to endorse or promote products derived from the Apple
			Software without specific prior written permission from Apple.  Except
			as expressly stated in this notice, no other rights or licenses, express
			or implied, are granted by Apple herein, including but not limited to
			any patent rights that may be infringed by your derivative works or by
			other works in which the Apple Software may be incorporated.
			
			The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
			MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
			THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
			FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
			OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
			
			IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
			OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
			SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
			INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
			MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
			AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
			STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
			POSSIBILITY OF SUCH DAMAGE.
*/
#include "AUEffectBase.h"
#include "SonogramViewDemoVersion.h"

#include "CARingBuffer.h"
#include "CABufferList.h"
#include "CASpectralProcessor.h"

#include "CASonogramViewSharedData.h"


#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __SonogramViewDemo_h__
#define __SonogramViewDemo_h__

// Here we define a custom property so the view is able to retrieve the spectrum overview
// graph. The graph changes often...
// custom properties id's must be 64000 or greater
// see <AudioUnit/AudioUnitProperties.h> for a list of Apple-defined standard properties
//
#pragma mark ___SonogramViewDemo Properties
enum
{
	kAudioUnitProperty_SonogramOverview = 65536,
	kAudioUnitProperty_SampleTimeStamp = 65537,
};


#pragma mark ____SonogramViewDemo Parameters

static const UInt64 kDefaultValue_BufferSize = kMaxNumAnalysisFrames*kMaxNumBins;



#pragma mark ____SonogramViewDemo
class SonogramViewDemoKernel : public AUKernelBase		// most real work happens here
{
	public:
		SonogramViewDemoKernel(AUEffectBase *inAudioUnit )
		: AUKernelBase(inAudioUnit){}
		
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
        virtual void		Reset();
		
		//private: //state variables...
};


class SonogramViewDemo : public AUEffectBase
{
public:
	SonogramViewDemo(AudioUnit component);

	virtual ~SonogramViewDemo (); 

	void	Cleanup();
		
	virtual AUKernelBase *		NewKernel() { return new SonogramViewDemoKernel(this); }
	
	void						AllocateBuffers();
	
	virtual	OSStatus			GetParameterValueStrings(	AudioUnitScope			inScope,
															AudioUnitParameterID	inParameterID,
															CFArrayRef *			outStrings);
    
	virtual	OSStatus			GetParameterInfo(	AudioUnitScope			inScope,
													AudioUnitParameterID	inParameterID,
													AudioUnitParameterInfo	&outParameterInfo);
    
	virtual OSStatus			GetPropertyInfo(	AudioUnitPropertyID		inID,
													AudioUnitScope			inScope,
													AudioUnitElement		inElement,
													UInt32 &				outDataSize,
													Boolean	&				outWritable );
	
	virtual OSStatus			GetProperty(	AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement 		inElement,
												void *					outData);
	
	virtual Float64				GetTailTime(){return(0.0);}
	virtual	bool				SupportsTail () { return true; }
	
	/*! @method Version */
	virtual OSStatus			Version() { return kSonogramViewDemoVersion; }
	
	virtual OSStatus			Initialize();
	
   	virtual	OSStatus			ChangeStreamFormat (	AudioUnitScope						inScope,
														AudioUnitElement					inElement,
														const CAStreamBasicDescription & 	inPrevFormat,
														const CAStreamBasicDescription &	inNewFormat);

		OSStatus				Render(			AudioUnitRenderActionFlags		&ioActionFlags,
												const AudioTimeStamp &			inTimeStamp,
												UInt32							inFramesToProcess );
												
		OSStatus				GetSonogramOverview(	SonogramOverview*		data);

	private:
		CARingBuffer*					mSpectrumBuffer;			// for storing the spectral data
		CABufferList*					mFetchingBufferList;		// for fetching from the ring buffer
		
		CABufferList*					mSpectralDataBufferList;	// for computing fft from the input
		
		AudioTimeStamp					mRenderStamp;				
		
		CAAutoDelete<CASpectralProcessor>	mSpectralProcessor;				
		UInt32								mBlockSize;					// nom. 1024
		UInt32								mNumBins;
		
		Float32*							mMinAmp;
		Float32*							mMaxAmp;
		
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif