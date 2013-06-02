/**
 * \class	DebugPageManager
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __DebugPageManager_H__
#define __DebugPageManager_H__

#define DEBUG_PAGES_ENABLED

// Includes
#include "Singleton.h"
#include "Array.h"
#include "StringId.h"
#include "TCanvas.h"

// External classes
class Bitmap;
class DebugPage;

// DebugPageCommand
enum DebugPageCommand
	{
	DebugPageCommand_Create,
	DebugPageCommand_Destroy,
	DebugPageCommand_Enter,
	DebugPageCommand_Exit,
	DebugPageCommand_Record,
	DebugPageCommand_Update,
	DebugPageCommand_Render,
	DebugPageCommand_Event,
	};

typedef void(*DebugPageHandlerFunction)(DebugPageCommand, void*, void**);

// DebugPageManager
class DebugPageManager : public Singleton<DebugPageManager>
	{
	public:
		DebugPageManager();
		~DebugPageManager();

		void SetBackground(TColor background = TLightGrey, unsigned char alpha = 224);

		void Update(float deltaTime);
		void Render(Bitmap& bitmap);

		void Register(StringId id, DebugPageHandlerFunction handler, void* instance);
		void Unregister(StringId id, DebugPageHandlerFunction handler, void* instance);

	private:
		void DrawCanvas(Bitmap& bitmap, TCanvas& canvas) const;


	private:
		TCanvas* canvas_;
		TColor background_;
		unsigned char backgroundAlpha_;
		int screenWidth_;
		int screenHeight_;
		int offsetX_;
		int offsetY_;
		bool isShowingDebugPage_;
		int closeX_;
		int closeY_;
		StringId currentDebugPage_;
		struct DebugPageEntry
			{
			StringId id;
			DebugPage* page;
			DebugPageHandlerFunction handler;
			void* instance;
			};
		Array<DebugPageEntry> pages_;
	};


#ifdef DEBUG_PAGES_ENABLED

	#define DEBUG_PAGE_BEGIN(className) \
		class className##_DebugPageRegistrar \
			{ \
			public: \
			className##_DebugPageRegistrar() \
				{ \
				if (DebugPageManager::IsInstanceCreated()) \
					{ \
					className* instance = (className*)(((unsigned char*)this) - (unsigned char*)&(((className *)0)->className##_debugPageRegistrar_)); \
					DebugPageManager::GetInstance()->Register(#className,className##_DebugPageHandler,instance); \
					} \
				} \
			~className##_DebugPageRegistrar() \
				{ \
				if (DebugPageManager::IsInstanceCreated()) \
					{ \
					className* instance = (className*)(((unsigned char*)this) - (unsigned char*)&(((className *)0)->className##_debugPageRegistrar_)); \
					DebugPageManager::GetInstance()->Unregister(#className,className##_DebugPageHandler,instance); \
					} \
				} \
			}; \
		className##_DebugPageRegistrar className##_debugPageRegistrar_; \
			static void className##_DebugPageHandler(DebugPageCommand command, void* instance, void** parameters) \
			{ \
			className* object = (className*)instance; 		

	#define DEBUG_PAGE_CREATE(method) \
			if (command==DebugPageCommand_Create) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				object->method(*page); \
				}

	#define DEBUG_PAGE_DESTROY(method)\
			if (command==DebugPageCommand_Destroy) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				object->method(*page); \
				} 

	#define DEBUG_PAGE_ENTER(method)\
			if (command==DebugPageCommand_Enter) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				object->method(*page); \
				} 

	#define DEBUG_PAGE_EXIT(method)\
			if (command==DebugPageCommand_Exit) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				object->method(*page); \
				} 

	#define DEBUG_PAGE_RECORD(method) \
			if (command==DebugPageCommand_Record) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				float* deltaTime = (float*)parameters[1]; \
				object->method(*page,*deltaTime); \
				} 

	#define DEBUG_PAGE_UPDATE(method) \
			if (command==DebugPageCommand_Update) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				float* deltaTime = (float*)parameters[1]; \
				object->method(*page,*deltaTime); \
				} 

	#define DEBUG_PAGE_RENDER(method)\
			if (command==DebugPageCommand_Render) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				TCanvas* canvas = (TCanvas*)parameters[1]; \
				object->method(*page,*canvas); \
				} 

	#define DEBUG_PAGE_EVENT(method) \
			if (command==DebugPageCommand_Event) \
				{ \
				DebugPage* page = (DebugPage*)parameters[0]; \
				StringId* eventId = (StringId*)parameters[1]; \
				object->method(*page,*eventId); \
				} 

	#define DEBUG_PAGE_END() \
			} 

#else

	#define DEBUG_PAGE_BEGIN(className)
	#define DEBUG_PAGE_CREATE(method)
	#define DEBUG_PAGE_DESTROY(method)
	#define DEBUG_PAGE_ENTER(method)
	#define DEBUG_PAGE_EXIT(method)
	#define DEBUG_PAGE_RECORD(method)
	#define DEBUG_PAGE_UPDATE(method)
	#define DEBUG_PAGE_RENDER(method)
	#define DEBUG_PAGE_EVENT(method)
	#define DEBUG_PAGE_END()

#endif

class Parent
	{
	public:
		Parent() {

			}
	virtual int c() {};
	};

class Parent2
	{
	public:
		Parent2() {

			}
	virtual int d() {};
	};


class TestManager:public Parent, Parent2
	{
	public:
		TestManager() {

			}
		void Update(float deltaTime) { }

		DEBUG_PAGE_BEGIN(TestManager)
			DEBUG_PAGE_CREATE(DebugPageCreate)
			DEBUG_PAGE_DESTROY(DebugPageDestroy)
			DEBUG_PAGE_ENTER(DebugPageEnter)
			DEBUG_PAGE_EXIT(DebugPageExit)
			DEBUG_PAGE_UPDATE(DebugPageUpdate)
			DEBUG_PAGE_RECORD(DebugPageRecord)
			DEBUG_PAGE_RENDER(DebugPageRender)
			DEBUG_PAGE_EVENT(DebugPageEvent)
		DEBUG_PAGE_END()

		void DebugPageCreate(DebugPage& page);
		void DebugPageDestroy(DebugPage& page);
		void DebugPageEnter(DebugPage& page);
		void DebugPageExit(DebugPage& page);
		void DebugPageUpdate(DebugPage& page, float deltaTime);
		void DebugPageRecord(DebugPage& page, float deltaTime);
		void DebugPageRender(DebugPage& page, TCanvas& canvas);
		void DebugPageEvent(DebugPage& page, StringId eventId);
	};



#endif /* __DebugPageManager_H__ */
