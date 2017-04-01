(function () {
    var App = angular.module("HMS", []);
    var HMSController = function ($scope, $http, $interval, $GetSensorsService, $ControlService) {
        $scope.SensorsTemperature = "";
        $scope.SensorsTemperature = "";
        $scope.ControlLights = "";
        $scope.ControlDoorLock = "";
        $scope.ControlWindows = "";
        $scope.SecondsToUpdate = 5;
        var DecrementSecondsToUpdate = function () {
            $scope.SecondsToUpdate -= 1;
            if ($scope.SecondsToUpdate === 0) {
                $scope.SecondsToUpdate = 5;
                $scope.UPDATE();
            }
        };
        var StartUpdating = function () {
            $scope.UPDATE();
            $interval(DecrementSecondsToUpdate, 1000);
        }
        $scope.DO = function () {
            $ControlService.SetControl($scope.Lights, $scope.DoorLock, $scope.Windows);
        };
        $scope.UPDATE = function () {
            $GetSensorsService.GetSensorsReadings().then(function (SensorsReadings) {
                $scope.SensorsTemperature = SensorsReadings.Temperature;
                if (SensorsReadings.PIR == "0")
                    $scope.SensorsPIR = "Empty";
                else
                    $scope.SensorsPIR = "Not Empty";
            });
            $ControlService.GetControl().then(function (ControlParameters) {
                if (ControlParameters[0] == "0")
                    $scope.ControlLights = "OFF";
                else
                    $scope.ControlLights = "ON";
                if (ControlParameters[1] == "0")
                    $scope.ControlDoorLock = "Closed";
                else
                    $scope.ControlDoorLock = "Opened";
                if (ControlParameters[2] == "0")
                    $scope.ControlWindows = "Closed";
                else
                    $scope.ControlWindows = "Opened";
            })
        };
        StartUpdating();
    };
    App.controller("HMSController", HMSController);
} ());