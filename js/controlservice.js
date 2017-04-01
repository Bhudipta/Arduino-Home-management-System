(function () {
    var $ControlService = function ($http) {
        var SetControl = function (Lights, DoorLock, Windows) {
            $http.get("<<server_ip>>/api/SetControl/Lights/" + Lights);
            $http.get("<<server_ip>>/api/SetControl/DoorLock/" + DoorLock);
            $http.get("<<server_ip>>/api/SetControl/Windows/" + Windows);
        };
        var GetControl = function () {
            return $http.get("<<server_ip>>/api/GetControl/").then(function (response) {
                return response.data;
            });
        };
        return {
            SetControl: SetControl,
            GetControl: GetControl
        };
    };
    var App = angular.module("HMS");
    App.factory('$ControlService', $ControlService);
} ());