#ifndef _FONT_H
#define _FONT_H
/*
 * =====================================================================================
 *        Class:  Font
 *  Description:  
 * =====================================================================================
 */
class Font
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Font (std::string name){_name=name;};                             /* constructor      */
		~Font (){};                            /* destructor       */

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */

		Font& operator = ( const Font &other ); /* assignment operator */

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		std::string _name;
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class Font  ----- */
#endif