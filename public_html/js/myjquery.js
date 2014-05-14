$(function() {
	$.getJSON('http://at01.cs.umb.edu:6801/collections', function(data){
	//console.log(data);
	}).done(function( data ) {
		var table_obj = $("#tr0");
		$.each( data.Collections, function( name, urlname, torrentcount, totalsizebytes, mirrored ) {
			var table_row = $('<tr class="' + this.name + '">' );
			var coll_id = this.urlname;
			var table_cell1 = $('<td id=' + coll_id +'>' + this.name + '</td>');
			var table_cell2 = $('<td>', {html: bytesToSize(this.totalsizebytes)});
			if(this.mirrored == 1){
				var table_cell3 = $('<td> <button id="unsubscribe" type="submit" class="btn btn-warning navbar-btn">Unsubscribe <span class="glyphicon glyphicon-remove"></span></button> </td>');
				}else{
					var table_cell3 = $('<td><button id="subscribe" type="submit" class="btn btn-success btn-block">subscribe</button></td>');
				}
			
//			var checkbox = $('<input type="checkbox" value="'+ coll_id + '"></input>');
//			console.log(this.mirrored + coll_id);
//	
//			
//			table_row.append(checkbox);
			table_row.append(table_cell1);
			table_row.append(table_cell2);
			table_row.append(table_cell3);
			table_obj.append(table_row);
	//		console.log(this.mirrored);
			if(this.mirrored == 1){
				console.log("hello red");
				$('#' + coll_id).html(this.name + " " +"[s]");
			}
	//		Elements.clickFunction(table_row, this.mirrored, this.name);
			//$('#subscribe').hide();
		
	});
	
	
	$('td').click(function(e){

		var urlName = $(this).attr('id');
		var COLLECTIONAPI = "http://at01.cs.umb.edu:6801/collections/" + urlName;
		//console.log(COLLECTIONAPI);
		
		
		$.ajax({
			type : 'GET',
			url: COLLECTIONAPI,
			dataType: 'json',
			success:function(data) {
				//console.log(data);
				console.log("success");
				$("#tbody1").empty();
				myFunction(urlName, data);
//				if(mirrored == 1){
//					$('#unsubscribe').show();
//				}else{
//					$('#subscribe').show();
//				}
			},
			complete :function(data) {
				console.log("complete");
			},
						error :function(data, error) {
				console.log("error" + error);
			}

		});
		console.log("finish");
 });

});

$("#subscribe").click(function(e){
	var urlname = $('#collection-name').html();
	var subscribe = confirm("Are you sure you want to subscribe to " + urlname + "?");
	console.log(urlname);
	if(subscribe){
	$.ajax({
		type : 'GET',
		url: "/subscribe/" + urlname,
		success:function(data) {
			console.log("success");
			$('#subscribe').hide();
			$('#unsubscribe').show();
			
		},
		complete :function(data) {
			console.log("complete");
			$('#' + coll_id).html(urlname + " " +"[s]");
			//$('#' + coll_id).html(this.name + " " +"[s]");
		},
					error :function(data, error) {
			console.log("error " + error);
		}

	});
	console.log("finish");
	}
	else
		{console.log("Not subscribed");}
});


// add url as class to button and add url as id to row, then use button class as click function and find row id using url

$("#unsubscribe").click(function(e){
	var urlname = $('#collection-name').html();
	var subscribe = confirm("Are you sure you want to unsubscribe to " + urlname + "?");
	console.log(urlname);
	if(subscribe){
	$.ajax({
		type : 'GET',
		url: "/unsubscribe/" + urlname,
		success:function(data) {
			console.log("success");
			//$("#unsubscribe").text("subscribe");
			$('#unsubscribe').hide();
			$('#subscribe').show();
		},
		complete :function(data) {
			console.log("complete");
			$('#' + coll_id).html(urlname);
		},
					error :function(data, error) {
			console.log("error " + error);
		}

	});
	console.log("finish");
	}
	else
		{console.log("Not subscribed");}
});

});

function bytesToSize(bytes) {
	// var sizes = [ '0','bytes', 'KB', 'MB', 'GB', 'TB'];
	// //bytes *= 1;
	// var i =+ Math.floor(Math.log(bytes) / Math.log(1024));
	// return  (bytes / Math.pow(1024, i)).toFixed( i ? 1 : 0 ) + ' ' + sizes[ i+1 ];

	var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
	var posttxt = 0;
	if(bytes === "0"){
		bytes = 0.0;
	}
//if (bytes == 0) return 0;
while( bytes >= 1024 ) { 
	posttxt++;
	bytes /= 1024;
}   
return  bytes.toFixed(1) +sizes[posttxt];
}


//var urlName = $('td').attr('id');
//var COLLECTIONAPI = 'http://at01.cs.umb.edu:6801/collections/' + urlName;
//console.log(COLLECTIONAPI);


	
//	$('td').click(function(e){
//		console.log("mani2");
//		var urlName = $(this).attr('id');
//		var urlName = $(this).attr('id');
//		var COLLECTIONAPI = "http://at01.cs.umb.edu:6801/collections/" + urlName;
//		console.log(COLLECTIONAPI);
//		$("#table1").empty();
////		$.ajax({
////			type : 'GET',
////			url: COLLECTIONAPI,
////			dataType: 'json',
////			success:function(data) {
////				console.log("success");
////				myFunction(urlName, data);
////			},
////			complete :function(data) {
////				console.log("complete");
////			},
////						error :function(data, error) {
////				console.log("error" + error);
////			},
////			data: "hello world"
////
////		});
//		console.log("finish");
// 	//myFunction(urlName);
// });


	function myFunction(collection_name, data){
		var table_obj = $('#table1');
		
	$.each( data[collection_name], function( name, sizebytes, type, mirrors, status, infohash, downloaders, filename, bibtex ) {
		var table_row = $('<tr>' );
		$('#collection-name').text(collection_name);
		
		
		
		var table_row2 = $('<tr>');
		var table_cell1 = $('<td href="#' + this.infohash + '"  data-toggle=collapse' + '>' + this.name + '</td>');
//		if ((this.status == 0 || this.status == 1) && this.filename != 'NULL'){
//			var table_cell1 = $('<td><a href= "/data/' + this.infohash + '/' + this.filename + '">' + this.name + '</a> </td>');
//		}else{
//			var table_cell1 = $('<td>', {html: this.name} );
//		}
		var table_cell2 = $('<td>' + "Size: " +  bytesToSize(this.sizebytes) + '</td>');
		var table_cell3 = $('<td>' + "mirrors: " + this.mirrors + '</td>');
		var table_cell4 = $('<td>' + "downloaders: " + this.downloaders + '</td>');
		//if ((this.status == 0 || this.status == 1) && this.filename != 'NULL'){
			var table_cell5 = $('<td>' + "files: " + '<a href= "/data/' + this.infohash + '/' + this.filename + '">' + this.filename + '</a>');
//		}else {
//			var table_cell5 = $('<td>', {html: NUll} ); }
		var table_cell6 = $('<textarea class="form-control" rows="5">' + this.bibtex + '</textarea>')
		var div_test = $('<div class="collapse" id="' + this.infohash + '"> </div>');
		
		
		table_row.append(table_cell1);
		
		div_test.append("<br>");
		div_test.append(table_cell2);
		div_test.append("<br>");
		div_test.append(table_cell3);
		div_test.append("<br>");
		div_test.append(table_cell4);
		div_test.append("<br>");
		div_test.append(table_cell5);
		div_test.append("<br>");
		div_test.append(table_cell6);
		div_test.append("<br>");
		table_row2.append(div_test);
		table_obj.append(table_row);
		table_obj.append(table_row2);
		
		$('.collapsing').collapse();
		
//		if ((this.status == 0 || this.status == 1) && this.filename != 'NULL'){
//			var table_cell1 = $('<td><a href= "/data/' + this.infohash + '/' + this.filename + '">' + this.name + '</a>');
//		}else{
//			var table_cell1 = $('<td>', {html: this.name} );
//		}
		
		
//		var table_cell2 = $('<td>', {html: bytesToSize(this.sizebytes)});
//		var table_cell3 = $('<td>', {html: this.type});
//		var table_cell4 = $('<td>', {html: this.mirrors});
//		var div_test = $('<div class="collapsing" id="' + this.infohash + '"> </div>');
//		
//		
//		var myRow = $('<td id="' + this.infohash +  '">' + this.name);
//		var myDiv = $('<div class="panel-collapse collapsing" id="' + this.infohash + '"> <br>' +  "size: " + bytesToSize(this.sizebytes) + "Mirrors: " + this.mirror + "Downloaders: " + this.downloaders + '</div>');
//		
//		
//		table_row.append(table_cell1);
//		table_row.append(myRow);
//		myRow.append(myDiv);
//		table_row.append(myDiv);
//		div_test.append("<br>");
//		div_test.append("Size: " + bytesToSize(this.sizebytes));
//		div_test.append("Mirrors: " + this.mirrors);
//		div_test.append("Downloaders: " + this.downloaders);
		
//		table_row.append(table_cell2);
//		table_row.append(table_cell3);
//		table_row.append(table_cell4);
//		table_obj.append(table_row);
		//console.log(this.name+ ' '+bytesToSize(this.size) + coll_id);
//		$('td').click(function(){
//			
//		});	
	
	});	
	
	}

	

	
	