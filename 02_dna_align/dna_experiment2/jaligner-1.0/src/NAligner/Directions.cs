#region NAligner Copyright
/*
 * NAligner
 * C# port of JAligner API, http://jaligner.sourceforge.net
 * 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#endregion

namespace NAligner
{
	// TODO: change Directions by an Enum

	/// <summary> Traceback directions. </summary>
	public abstract class Directions
	{
		/// <summary> Traceback direction stop</summary>
		public const byte STOP = 0;
		/// <summary> Traceback direction left</summary>
		public const byte LEFT = 1;
		/// <summary> Traceback direction diagonal</summary>
		public const byte DIAGONAL = 2;
		/// <summary> Traceback direction up</summary>
		public const byte UP = 3;
	}
}