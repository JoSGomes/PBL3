<?php

namespace App\Http\Livewire;

use Livewire\Component;
use PhpMqtt\Client\Facades\MQTT;
use App\Models\Configuration;

class ConfigComponent extends Component
{

    public $intervalConnection; 
    public $timeUnitConnection = 1;
    public $intervalAlarm;
    public $timeUnitAlarm = 1;
    public $sucessMessage;

    protected $rules = [
        'intervalConnection' => 'required|integer|gt:0',
        'intervalAlarm' => 'required|integer|gt:0',
        'timeUnitConnection' => 'required',
        'timeUnitAlarm' => 'required',
    ];

    protected $messages = [
        'intervalConnection.required' => 'O campo de tempo de verificação é necessário',
        'intervalConnection.integer' => 'O campo de tempo de verificação deve ser um inteiro',
        'intervalConnection.gt:0' => 'O campo de tempo de verificação deve ser maior que zero',
        'intervalAlarm.required' => 'O campo de tempo de acionamento é necessário',
        'intervalAlarm.integer' => 'O campo de tempo de acionamento deve ser um inteiro',
        'intervalAlarm.gt:0' => 'O campo de tempo de acionamento deve ser maior que zero',
    ];


    public function mount()
    {
        $config = Configuration::all()->last();
        if ($config) {
            $this->intervalConnection = $config->intervalConnection;
            $this->timeUnitConnection = $config->timeUnitConnection;
            $this->intervalAlarm = $config->intervalAlarm;
            $this->timeUnitAlarm = $config->timeUnitAlarm;
        }
    }


    public function updated($propertyName)
    {
        $this->validateOnly($propertyName);
    }

    public function setConfig(){
        
        //validação
        $validatedData = $this->validate();

        //enviando tópicos pra placa
        $mqtt = MQTT::connection();
        $mqtt->publish('INTERVALO_SITE_CONNECTION',$validatedData['intervalConnection']*$validatedData['timeUnitConnection'], 0);  
        $mqtt->disconnect();

        $mqtt = MQTT::connection();
        $mqtt->publish('INTERVALO_SITE_ALARM',$validatedData['intervalAlarm']*$validatedData['timeUnitAlarm'], 0);  
        $mqtt->disconnect();

        //salvando na db
        Configuration::create($validatedData);
        
        //mensagem de sucesso
        $this->sucessMessage = 'Configurações alteradas!';
    }

    public function resetMessage() {
        $this->sucessMessage = null;
    }

}
