#ifndef DistanceMatrixReader_H
#define DistanceMatrixReader_H

#include "DelimitedDataReader.h"
#include "MatrixReal.h"
#include <string>
#include <vector>

namespace RevBayesCore {
	
	
	/**
	 * Reader for distance matrices from Phylip format files.
	 * The first line contains the number of tips.
	 * The following lines contain pairwise distances.
	 *
	 *
	 *
	 * @copyright Copyright 2009-
	 * @author The RevBayes Development Core Team (Bastien Boussau)
	 * @since 2015-03-03, version 1.0
	 *
	 */
	class DistanceMatrixReader : public DelimitedDataReader {
		
	public:
		
		DistanceMatrixReader(const std::string &fn, char d='\t', size_t ns=0);
		
		const std::vector<std::string>&                 getNames(void);
		const MatrixReal&   							getMatrix(void);
		
		
		
	protected:
		
		std::vector<std::string>                        names;
		MatrixReal          							matrix;
		
	};
	
}

#endif