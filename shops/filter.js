function obeSearch(textToFind) {
	var selectBarrio	= document.getElementById('selectBarrio');
	var selectedOption	= selectBarrio.selectedIndex;
	var selectedBarrio	= selectedOption - 1;
	var lowerToFind		= textToFind.toLowerCase();
	var wordsToFind		= lowerToFind.split(' ');
	var pos				= 0;
	for(pos = 0; pos < idList.length; pos = pos +1)
		document.getElementById(idList[pos]).style.visibility = 'collapse';
	for(pos = 0; pos < names.length; pos = pos +1) {
		for(toFind of wordsToFind) {
			if(' ' === toFind || '' === toFind)
				continue;
			if(names[pos].indexOf(toFind) > -1) {
				var idc = 0;
				for(idc = 0; idc < indices[pos].length; idc++)
					var idArticle = indices[pos][idc];
					if(selectedBarrio != '-1') {
						var iBarrio = 0;
						for(iBarrio = 0; iBarrio < barrioIdMap[idArticle].length; iBarrio++) {
							if(selectedBarrio == barrioIdMap[idArticle][iBarrio]) {
								document.getElementById(idArticle).style.visibility = 'visible';
								break;
							}
						}
					}
					else
						document.getElementById(idArticle).style.visibility = 'visible';
			}
		}
	}
}
function clearSearch() {
	var selectBarrio	= document.getElementById('selectBarrio');
	var selectedOption	= selectBarrio.selectedIndex;
	var selectedBarrio	= selectedOption - 1;
	if(selectedBarrio == '-1') {
		for(id of idList)
			document.getElementById(id).style.visibility = 'visible';
	}
	else {
		for(id of idList) {
			var articleElement	= document.getElementById(id);
			articleElement.style.visibility = 'collapse';
			var barrioMap		= barrioIdMap[id];
			for(barrio of barrioMap) {
				if(selectedBarrio == barrio) {
					articleElement.style.visibility = 'visible';
					break;
				}
			}
		}
	}
}
