function copy(){
	var org_username = document.getElementById("orgUsername");
	var copy_username = document.getElementById("copyUsername");
	if(org_username.value == "" || org_username.value == "Please fill out" || org_username.value == "Enter Username"){
		document.getElementById("error").setAttribute("class", "form-group has-error");
		org_username.value = "Please fill out";
		return false;
	} else {
		document.getElementById("error").setAttribute("class", "form-group");
		copy_username.value = org_username.value;
	}

}

function clear_input(){
	var org_username = document.getElementById("orgUsername");

	if(org_username.value == 'Please fill out'  || org_username.value == "Enter Username"){
		org_username.value = '';
	}
}

function reset(){
		document.getElementById("error").setAttribute("class", "form-group");
		copy_username.value = org_username.value;
}