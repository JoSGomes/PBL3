<?php

namespace App\Http\Livewire;

use Livewire\Component;
use App\Models\Event;

class EventTable extends Component
{
    public $events;
    public $day;
    public $month;

    public function render()
    {
        return view('livewire.event-table');
    }

    public function search(){
        $this->events = Event::where('month',$this->month)->where('day',$this->day)->get();
    }

    public function mount(){
        
        $lastEvent = Event::all()->last();
        if ($lastEvent) {
            $this->month = $lastEvent->month;
            $this->day =  $lastEvent->day;
            $this->events = $lastEvent->get();
        }
        
    }
}
