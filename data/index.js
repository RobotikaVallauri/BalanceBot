var esempioSocket;
$(document).ready(function() {
	esempioSocket = new WebSocket('ws://' + window.location.hostname + ':81/'); 	
    $("#btn-grafico").click(function(){
		CambiaPagina(this,"grafico.html");
	});
	$("#btn-gioco").click(function(){
		CambiaPagina(this,"traffic.html");
	});
	$("#btn-reset").click(function(){
		ResetSensore();
	});
});

function CambiaPagina(btnpremuto,nuovapagina)
{	
	$("#main-menu").find(".active-menu").removeClass("active-menu");
	$(btnpremuto).addClass("active-menu");
	$("#iframe-pagina").attr("src",nuovapagina);
	//$(".navbar-toggle").trigger("click");
}

function ResetSensore()
{
	   	
	esempioSocket.send("0");
											
}