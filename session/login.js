function							min								(a, b)		{ if(a < b) return a; else return b; }
function							qrn								(x)			{ x = (x << 13) ^ x; return ( x * (x * x * 15661 + 715827883)  + 1500450271); }
function							qdg								(str)		{
	var										x								= 0;
	var										i								= 0;
	var										filtered						= [];
	for(i = 0; i < str.length - 8; i++) {
		x	+= qrn(str[i])
			+  qrn(str[i + 1])
			+  qrn(str[i + 2])
			+  qrn(str[i + 3])
			+  qrn(str[i + 4])
			+  qrn(str[i + 5])
			+  qrn(str[i + 6])
			+  qrn(str[i + 7])
			;
		x									+= x ^ (x << 11);
		filtered.push(x);
	}
	for(i = 0; i < filtered.length - 8; i++) {
		filtered[i]	^= qrn(filtered[i])
					+  qrn(filtered[i + 1])
					+  qrn(filtered[i + 2])
					+  qrn(filtered[i + 3])
					+  qrn(filtered[i + 4])
					+  qrn(filtered[i + 5])
					+  qrn(filtered[i + 6])
					+  qrn(filtered[i + 7])
					;
	}
	for(i = 2; i < (filtered.length - 8); i += 2) {
		for(j = 0; j < 8; j++)
			filtered[j]							+= filtered[i + j];
	}
	result								= "";
	for(i = 0; i < min(filtered.length, 8); i++)
		result								+= filtered[i].toString();
	return result;
}

function							formOnClickSubmit				(signup)	{
	var										inputCheckUser					= document.getElementById('user_used');
	var										inputCheckPass					= document.getElementById('pass_used');
	if (inputCheckUser.value === '0' || inputCheckPass.value === '0')
		return false;
	var										formLogin						= document.getElementById('login');
	var										inputUser						= document.getElementById('user');
	var										inputPass						= document.getElementById('pass');
	if(signup) {
		var										inputTest						= document.getElementById('test');
		if(inputPass.value != inputTest.value) {
			var										inputTestRow					= document.getElementById('test_row');
			inputTestRow.style.visibility		= "visible";
			return false;
		}
	}
	var										inputb64User					= document.getElementById('b64user');
	var										inputb64Pass					= document.getElementById('b64pass');
	var										lowerUser						= inputUser.value.toLowerCase();
	inputb64User.value					= btoa(lowerUser);
	inputb64Pass.value					= btoa(qdg(inputPass.value));
	formLogin.submit();
}
