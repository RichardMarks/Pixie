/**
 * \class	SpriteAction
 *
 * \ingroup	graphics
 * \brief
 * \author	Mattias Gustavsson
 *
 *
 * \todo	Refactor/Redesign/Cleanup (structs?)
 * \todo	Make sure origin is supported for all actions
 * \todo	Sprite action for animation
 */

#ifndef __SpriteAction_H__
#define __SpriteAction_H__

// Includes

// Forward declares
class Sprite;
class SpriteController;

namespace pixie {

// SpriteAction
class SpriteAction
	{
	public:
		SpriteAction();

		virtual ~SpriteAction() { };

		virtual void Update(SpriteController* controller, Sprite* sprite, float deltaTime) = 0;

		virtual void Setup(SpriteController* controller, Sprite* sprite) { };

		void ActionCompleted();

		bool IsActionCompleted() const;

	private:
		bool actionCompleted_;
	};

}; // namespace pixie

#endif /* __SpriteAction_H__ */

