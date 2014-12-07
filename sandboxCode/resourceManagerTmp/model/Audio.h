#ifndef _AUDIO_H
#define _AUDIO_H
#include <iostream>
/*
 * =====================================================================================
 *        Class:  Audio
 *  Description:  
 * =====================================================================================
 */

static void free_audio(Audio *a) }{
	std::cout << "Freeing audio" << std::endl;
}
class Audio
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Audio (std::string name){_name=name;};                             /* constructor      */
		~Audio (){};                            /* destructor       */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

		Audio& operator = ( const Audio &other ); /* assignment operator */

		void print() {
			std::cout << _name << std::endl;
		}
	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		std::string _name;
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class Audio  ----- */
#endif
