function							cellColor						(idCell, color_back, color_font) {
    var										cell							= document.getElementById(idCell);
	cell.style.backgroundColor			= color_back;
	cell.style.color					= color_font;
}

function							reframe							(namframe, m, p, l, s) {
	var										inputM							= document.getElementById('m');
	inputM.setAttribute('value', m);
	var										inputP							= document.getElementById('p');
	inputP.setAttribute('value', p);
	var										inputL							= document.getElementById('l');
	inputL.setAttribute('value', l);
	var										inputS							= document.getElementById('s');
	inputS.setAttribute('value', s);
	var										frameElement					= document.getElementById(namframe);
	var										frameRect						= frameElement.getBoundingClientRect();
	frameElement.setAttribute("src"
		, p
		+ "?m="
		+ m
		+ "&h="
		+ frameRect.height
		+ "&w="
		+ frameRect.width
		+ "&l="
		+ l
		+ "&s="
		+ s
		);
}

function							setLang							(language) {
	//var										inputPricing					= document.getElementById('pricing'	);
	var										inputTourism					= document.getElementById('tours'	);
	var										inputShow	  					= document.getElementById('shows'	);
	var										inputShopping  					= document.getElementById('shops'	);
	var										inputMeals  					= document.getElementById('meals'	);
	if(language === 'es') {
		inputTourism	.innerHTML = "Turismo y<br/> Guías"		;
		inputShopping	.innerHTML = "Comercio y<br/> Servicios"	;
		inputShow		.innerHTML = "Shows y<br/> Arte"			;
		inputMeals		.innerHTML = "Comidas y<br/> Snacks"		;
		//inputPricing	.innerHTML = "Publicar"				;
	}
	else {
		inputTourism	.innerHTML = "Tourism &<br/> Guides"		;
		inputShopping	.innerHTML = "Shopping &<br/> Services"	;
		inputShow		.innerHTML = "Shows &<br/> Arts"			;
		inputMeals		.innerHTML = "Meals &<br/> Snacks"		;
		//inputPricing	.innerHTML = "Advertise"			;
	}
}
