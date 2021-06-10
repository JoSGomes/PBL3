<?php

namespace App\Http\Livewire;

use Livewire\Component;
use PhpMqtt\Client\Facades\MQTT;
use App\Models\Configuration;

class ConfigComponent extends Component
{

    public $intervalConnection = 1; 
    public $timeUnitConnection = 60;
    public $intervalAlarm = 1;
    public $timeUnitAlarm = 60;
    public $intervalBem = 1;
    public $timeUnitBem = 60;
    public $sucessMessage;


    protected $rules = [
        'intervalConnection' => 'required|integer|gt:0',
        'intervalAlarm' => 'required|integer|gt:0',
        'timeUnitConnection' => 'required',
        'timeUnitAlarm' => 'required',
        'intervalBem' => 'required|integer|gt:0',
        'timeUnitBem' => 'required'
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
            $this->intervalBem = $config->intervalBem;
            $this->timeUnitBem = $config->timeUnitBem;
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
        $mqtt->publish('INTERVAL_CONNECTION',$validatedData['intervalConnection']*$validatedData['timeUnitConnection'], 0); 
        $mqtt->publish('INTERVAL_ALARM',$validatedData['intervalAlarm']*$validatedData['timeUnitAlarm'], 0);          
        $mqtt->publish('INTERVAL_IMOK',$validatedData['intervalBem']*$validatedData['timeUnitBem'], 0);  
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
