(function () {

    var $GetSensorsService = function ($http) {
        var GetSensorsReadings = function () {
            return $http.get("<<Insert hosting/website domain or local host>>/api/GetSensors/").then(function (response) {
                return response.data;
            });
        };
        return {
            GetSensorsReadings: GetSensorsReadings
        };
    };

    var App = angular.module("HMS");
    App.factory("$GetSensorsService", $GetSensorsService);
} ());