// Create the chart
var chart=Highcharts.stockChart('container', {
  chart: {
    inverted: true,
    type: 'areaspline',
    events: {
      load: function () {

        // set up the updating of the chart each second
        var series = this.series[0];
        setInterval(function () {
          var x = (new Date()).getTime(); // current time
          var y = Math.round((Math.random() * 100)-50);
          //series.addPoint([x, y], false, false,false);
        }, 200);
      }
    }	
  },
	yAxis: {
        max: 50,
		min: -50,
		allowDecimals: true
    },
	navigator:
	{
		yAxis: {
        max: 50,
		min: -50,
		allowDecimals: true
    }
	},
  time: {
    useUTC: false
  },

  rangeSelector: {
    buttons: [
	{
      count: 30,
      type: 'second',
      text: '30S'
    },
	{
      count: 1,
      type: 'minute',
      text: '1M'
    }, {
      count: 5,
      type: 'minute',
      text: '5M'
    }, {
      type: 'all',
      text: 'All'
    }],
    inputEnabled: false,
    selected: 0
  },

  title: {
    text: 'Live random data'
  },

  exporting: {
    enabled: true
  },

  series: [{
    name: 'Random data',
    data: (function () {
      // generate an array of random data
      var data = [],
        time = (new Date()).getTime(),
        i;

      for (i = -999; i <= 0; i += 1) {
        data.push([
          time + i * 1000,
          0
        ]);
      }
      return data;
    }())
  }]
});

setInterval(function() {
  chart.redraw(false)
}, 1000/60)

$( document ).ready(function() {
   
   var esempioSocket = new WebSocket('ws://' + window.location.hostname + ':81/'); 
	esempioSocket.onmessage = function (event) {
		var valore;
		if(event.data<=180)
		{
			valore=event.data;
		}else
		{
			valore=event.data-360;

		}
		console.log(valore);
		var series = chart.series[0];
		var x = (new Date()).getTime(); // current time
        var y = Math.round(valore);
         series.addPoint([x, y], false, false,false);
											}
   
   
});
