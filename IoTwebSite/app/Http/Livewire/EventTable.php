<?php

namespace App\Http\Livewire;

use Livewire\Component;
use App\Models\Event;

class EventTable extends Component
{
    public function render()
    {
        $events = Event::all();
        return view('livewire.event-table',compact('events'));
    }
}
