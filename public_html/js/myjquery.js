$(function(){
$.getJSON('http://at01.cs.umb.edu:6801/collections', function(data){
	//console.log(data);
}).done(function( data ) {
	var table_obj = $("#tr0");
	$.each( data.Collections, function( name, totalsizebytes, urlname, torrent_count ) {
		var table_row = $('<tr>' );
		var coll_id = this.urlname;
		var table_cell1 = $('<td id=' + coll_id +'>' + this.name + '</td>');
		var table_cell2 = $('<td>', {html: bytesToSize(this.totalsizebytes)});
		var checkbox = $('<input type="checkbox" value="'+ coll_id + '"></input>');
		

		//table_row.append(checkbox);
		table_row.append(table_cell1);
		table_row.append(table_cell2);
		table_obj.append(table_row);
		//console.log(this.name+ ' '+bytesToSize(this.totalsizebytes) + coll_id);
	});
	//console.log($('td'));
	
	$('td').click(function(e){

		var urlName = $(this).attr('id');
		var COLLECTIONAPI = "http://at01.cs.umb.edu:6801/collections/" + urlName;
		//console.log(COLLECTIONAPI);
		
		$.ajax({
			type : 'GET',
			url: COLLECTIONAPI,
			dataType: 'json',
			success:function(data) {
				console.log("success");
				//console.log(data);
				$("#tbody1").empty();
				myFunction(urlName, data);
			},
			complete :function(data) {
				console.log("complete");
			},
						error :function(data, error) {
				console.log("error" + error);
			}

		});
		console.log("finish");
 	//myFunction(urlName);
 });
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
return  bytes.toFixed(1) + ' '+sizes[posttxt];
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
		console.log(data);
	$.each( data[collection_name], function( name, sizebytes, type, mirrors, status, infohash, filename ) {
		var table_row = $('<tr>' );
		//var coll_id = this.urlname;
		//var table_link = $('<a href=" #">' );
		console.log(this.infohash);
		console.log(this.status);
		console.log(this.filename);
		if ((this.status == 0 || this.status == 1) && this.filename != 'NULL'){
			//var table_cell1 = $('<td><a href= "/data/' + this.infohash + '/' + this.filename + '">' + this.name + '</a>');
			var table_cell1 = $('<td><a href= "/data/' + this.infohash + '/' + this.filename + '">' + this.name + '</a>');
		}else{
			var table_cell1 = $('<td>', {html: this.name} );
		}
		
		
		var table_cell2 = $('<td>', {html: bytesToSize(this.sizebytes)});
		var table_cell3 = $('<td>', {html: this.type});
		var table_cell4 = $('<td>', {html: this.mirrors});

		//var checkbox = $('<input type="checkbox" value="'+ coll_id + '"></input>');
		
		$('#collection-name').text(collection_name);
		//table_row.append(checkbox);
		//table_obj.append(table_link);
		table_row.append(table_cell1);
		table_row.append(table_cell2);
		table_row.append(table_cell3);
		table_row.append(table_cell4);
		table_obj.append(table_row);
		//console.log(this.name+ ' '+bytesToSize(this.size) + coll_id);
	});	
	}