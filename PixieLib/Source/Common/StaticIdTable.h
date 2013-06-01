/**
 * \class	IdTable
 * 
 * \ingroup	containers
 * \brief	
 * \author	Mattias Gustavsson
 * 
 */

#ifndef __StaticIdTable_H__
#define __StaticIdTable_H__

// Includes
#include "PodCheck.h"
#include "StandardLibrary.h"

// Forward declares
typedef unsigned int Id;

// IdTable
template< typename T, unsigned int capacity_, POD_ENUM POD = NOT_POD, unsigned char COUNTER_BIT_COUNT = 12, unsigned char INDEX_BIT_COUNT = 20 >
class StaticIdTable
    {
    public:
        //*** Constructor ***
        
        StaticIdTable():
            count_( 0 ),
            usedIndicesCount_( 0 ),
            freeListHead_( 0 ),
            freeListTail_( 0 )
            {
            COMPILE_CHECK_IS_POD( T, POD );
            }
            
            
        //*** Destructor ***
        
        ~StaticIdTable()
            {
            if( POD == NOT_POD )
                {
                T* op = (T*)objects_;
                for( unsigned int i = 0; i < count_; i++ )
                    {
                    op->~T();
                    op++;
                    }
                } 
            }


        //*** Add ***

        Id Add( const T& object = T() )
            {
            if( count_ >= capacity_ )
                {
                Assert( false, "Capacity reached" );
                return ToId( (unsigned int)-1, (unsigned int)-1 );
                }

            unsigned int newIndex = (unsigned int)-1;
            if( usedIndicesCount_ >= capacity_ )
                {
                Assert( freeListHead_ != freeListTail_, "Freelist corrupted" );
                newIndex = freeList_[ freeListHead_ ];
                freeListHead_++;
                if( freeListHead_ >= capacity_ )
                    {
                    freeListHead_ = 0;
                    }
                }
            else
                {
                newIndex = usedIndicesCount_;
                usedIndicesCount_++;
                indices_[ newIndex ].counter = 0;
                }

            indices_[ newIndex ].objectIndex = count_;
            
            if( POD == NOT_POD )
                {
                new (&(((T*)objects_)[ count_ ]))  T( object );
                }
            else
                {
                ((T*)objects_)[ count_ ] = object;
                }
            objectSlotToIndex_[ count_ ] = newIndex;
            
            Id id = ToId( indices_[ newIndex ].counter, newIndex );
            
            count_++;
            return id;
            }

            
        //*** Remove ***

        void Remove( Id id )
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex >= capacity_ )
                {
                return;
                }
                
            Index index = indices_[ idIndex ];
            
            if( index.counter != idCounter || index.objectIndex >= count_ )
                {
                return;
                }
            
            ((T*)objects_)[ index.objectIndex ].~T();
            if( count_ > 0 && index.objectIndex < count_ - 1 )
                {
                if( POD == NOT_POD )                
                    {
                    new ( &((T*)objects_)[ index.objectIndex ]) T( ((T*)objects_)[ count_ -1 ] );
                    ((T*)objects_)[ count_ - 1 ].~T();
                    }
                else
                    {
                    ((T*)objects_)[ index.objectIndex ] = ((T*)objects_)[ count_ - 1 ];
                    }
                
                indices_[ objectSlotToIndex_[ count_ - 1 ] ].objectIndex = indices_[ objectSlotToIndex_[ index.objectIndex ] ].objectIndex;  
                objectSlotToIndex_[ index.objectIndex ] =  objectSlotToIndex_[ count_ -1 ];
                }
                
            count_--;                
            
            indices_[ idIndex ].counter++;
            freeList_[ freeListTail_ ] = idIndex;
            freeListTail_ ++;
            if( freeListTail_ >= capacity_ )
                {
                freeListTail_ = 0;
                }
            }

        
        //*** Get ***

        const T& Get( Id id ) const
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    return ((T*)objects_)[ index.objectIndex ];
                    }
                }
                    
            Assert( false, "Invalid id");                    
            static T defaultValue;
            return defaultValue; 
            }


        //*** Get ***

        T& Get( Id id )
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    return ((T*)objects_)[ index.objectIndex ];
                    }
                }
                    
            Assert( false, "Invalid id");                    
            static T defaultValue;
            return defaultValue; 
            }


        //*** Set ***

        void Set( Id id, const T& object )
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    ((T*)objects_)[ index.objectIndex ] = object;
                    }
                }
                
            }


        //*** IsValid ***

        bool IsValid( Id id ) const
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    return true;
                    }
                }
            
            return false;
            }
            

        //*** GetItemCount ***

        int GetItemCount() const
            {
            return count_;
            }           


        //*** Get ***

        const T& Get( int index ) const
            {
            if ( index < 0 || (unsigned int) index >= count_ )
                {
                Assert( false, "Index out of range");                    
                static T defaultValue;
                return defaultValue; 
                }
            return ((T*)objects_)[ index ];            
            }           


        //*** Get ***

        T& Get( int index )
            {
            if ( index < 0 || (unsigned int) index >= count_ )
                {
                Assert( false, "Index out of range");                    
                static T defaultValue;
                return defaultValue; 
                }
            return ((T*)objects_)[ index ];            
            }           


        //*** GetId ***

        Id GetId( int index ) const
            {
            if ( index < 0 || (unsigned int) index >= count_ )
                {
                Assert( false, "Index out of range");                    
                Id id = ToId( (unsigned int)-1, (unsigned int)-1 );
                return id;
                }
                
            unsigned int actualIndex = objectSlotToIndex_[ index ];                
            Id id = ToId( indices_[ actualIndex ].counter, actualIndex );
            return id;            
            }           

    private:

        //*** ToId ***

        Id ToId( unsigned int counter, unsigned int index ) const
            {
            const unsigned int COUNTER_BIT_MASK = ( 1 << COUNTER_BIT_COUNT ) - 1;
            const unsigned int INDEX_BIT_MASK = ( 1 << INDEX_BIT_COUNT ) - 1;

            Id id = index & INDEX_BIT_MASK;
            id |= ( counter & COUNTER_BIT_MASK ) << INDEX_BIT_COUNT;
            return id;
            }

        
        //*** FromId ***

        void FromId( Id id, unsigned int& counter, unsigned int& index ) const
            {
            const unsigned int COUNTER_BIT_MASK = ( 1 << COUNTER_BIT_COUNT ) - 1;
            const unsigned int INDEX_BIT_MASK = ( 1 << INDEX_BIT_COUNT ) - 1;

            counter = ( id  >> INDEX_BIT_COUNT ) & COUNTER_BIT_MASK;
            index = id & INDEX_BIT_MASK;
            }
            
    private:      
        struct Index
            {
            unsigned int counter : COUNTER_BIT_COUNT;
            unsigned int objectIndex : INDEX_BIT_COUNT;
            };
            
        unsigned int count_;
	    unsigned char* objects_[ sizeof( T ) * capacity_ ];
	    Index indices_[ capacity_ ];
	    unsigned int objectSlotToIndex_[ capacity_ ];
	    unsigned int freeList_[ capacity_ ];
	    unsigned int freeListHead_;
	    unsigned int freeListTail_;
        unsigned int usedIndicesCount_;
    };

// Implementation
#include "StaticIdTable.inl"

#endif /* __StaticIdTable_h__ */
