/**
 * \class	FrameTime
 *
 * \ingroup	core
 * \brief
 * \author	Mattias Gustavsson
 *
 *
 */

#ifndef __FrameTime_H__
#define __FrameTime_H__

// Includes

namespace pixie {

// Forward declares

// FrameTime
class FrameTime
	{
	public:
		FrameTime();

		float Update();
		float GetDeltaTime();
		int GetFrameCounter();

	private:
		bool initialized_;
		float previousFrameTime_;
		float deltaTime_;
		int frameCounter_;
	};

}; // namespace pixie

#endif /* __FrameTime_H__ */

